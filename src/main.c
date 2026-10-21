#include <stdio.h> // 입출력하려면 필수
#include <stdlib.h> // 동적 할당용
#include <time.h> // 나중에 코드 실행시간 측정용으로 넣음
#include <limits.h> // LLONG_MAX 용

#include "graph.h" // 그래프 
#include "hashTable.h" // 해시테이블 -> 유저 프로필(by. userID) or 게시물(by. hashTag) 검색
#include "generatorUtil.h" // 생성기 중복 함수 제거용
#include "dijkstra.h" // 다익스트라 (in graph) (with minHeap)
#include "recommend.h" // 추천친구 (in graph)
#include "maxHeap.h" // 트렌드 피드 순위 추출용
#include "ranking.h" // 병합 정렬, 친구수 top 50 출력용

#define EDGE_FILE "data/edge.txt"
#define PROFILE_FILE "data/profiles.txt"
#define HASHTAG_FILE "data/hashTags.txt"
#define POST_FILE "data/posts.txt"

int main(){
    GeneratorConfig config;

    // =================== 데이터 초기화 ======================
    
    // =================== 설정 파일 열기 =====================
    if(LoadGeneratorConfig(&config) != 0){
        return 1;
    }

    // ============ Graph 생성 + edge.txt 파일 읽기 ===========
    Graph* graph = CreateGraph(config.user);
    if(graph == NULL){
        printf("Failed to create graph.\n");
        return 1;
    }
    FILE* edgeFile = fopen(EDGE_FILE, "r");
    if(edgeFile == NULL) { 
        printf("Failed to read edge.txt\n");
        FreeGraph(graph); // 그래프 동적 할당 해제
        return 1; // 프로그램 중단
    }

    long long user1, user2;

    // edge.txt의 간선 데이터를 그래프에 삽입
    while(fscanf(edgeFile, "%lld %lld", &user1, &user2) == 2) {
        // 읽어온 두 유저 사이의 간선 추가 (무방향임 AddEdge()에서 중복이면 알아서 처리함)
        AddEdge(graph, user1, user2);
    }

    fclose(edgeFile); // 파일 읽기 종료
    
    // ===== UserHashTable 생성 + profiles.txt 파일 읽기 =====
    UserHashTable* userTable = CreateUserHashTable((int)(config.user/2)); // 임시로 테이블 크기는 유저수의 반절로 설정
    if(userTable == NULL){
        printf("Failed to create user hash table.\n");
        // 앞에 동적 할당한것들도 free 해줘야함
        FreeGraph(graph);
        return 1;
    }

    // profiles.txt 열기
    FILE* profileFile = fopen(PROFILE_FILE, "r");
    if(profileFile == NULL){
        printf("Failed to read profile file. (%s)\n", PROFILE_FILE);
        // 앞에 동적 할당한것들도 free 해줘야함
        FreeUserHashTable(userTable);
        FreeGraph(graph);
        return 1;
    }

    // userTable에 프로필 데이터 읽어서 삽입
    UserProfile profile;
    while(fscanf(profileFile, "%lld %d %19s %19s %19s", &profile.userID, &profile.age, profile.region, profile.interest, profile.status) == 5){
        // 삽입 실패
        if(InsertProfile(userTable, profile) != 0){
            printf("Failed to insert profile.\n");
            fclose(profileFile); // 파일 닫기
            FreeUserHashTable(userTable); // 할당 해제
            FreeGraph(graph);
            return 1;
        }
    }
    fclose(profileFile);

    // === HashTagHashTable 생성 + hashTags.txt 파일 읽기 ===
    FILE* hashTagFile = fopen(HASHTAG_FILE, "r");

    if(hashTagFile == NULL){
        printf("Failed to open file. (%s)\n", HASHTAG_FILE);
        FreeUserHashTable(userTable);
        FreeGraph(graph);
        return 1;
    }

    HashTagHashTable* hashTagTable = CreateHashTagHashTable(101); // 현재 생성되는 해시태그 key가 100종 -> 버킷 수 101개 (최종 제출전 수정예정)

    if(hashTagTable == NULL){
        printf("Failed to create hashtag hash table.\n");
        fclose(hashTagFile);
        FreeUserHashTable(userTable);
        FreeGraph(graph);
        return 1;
    }

    long long postID;
    int hashTagID;
    char hashTagName[50];

    // hashTagName[50] 에 끝부분은 \0 이라 49 까지만 읽어야함 안그럼 에러 뜸
    while(fscanf(hashTagFile, "%lld %d %49s", &postID, &hashTagID, hashTagName) == 3){
        if(InsertHashTag(hashTagTable, hashTagName, postID) != 0){
            printf("Failed to insert hashtag.\n");
            fclose(hashTagFile);
            FreeUserHashTable(userTable);
            FreeHashTagHashTable(hashTagTable);
            FreeGraph(graph);

            return 1;
        }
    }
    fclose(hashTagFile);

    // ======= posts.txt 파일 읽기 + 원본 게시물 저장 =======
    FILE* postFile = fopen(POST_FILE, "r"); // postID userID title content createdTime likes

    if(postFile == NULL){
        printf("Failed to open post file. (%s)\n", POST_FILE);
        FreeHashTagHashTable(hashTagTable);
        FreeUserHashTable(userTable);
        FreeGraph(graph);
        return 1;
    }
    
    // 트렌드 점수를 새로 갱신하기 위해 게시물의 원본 데이터를 배열에 저장함
    MaxHeapNode* trendPosts = (MaxHeapNode*)malloc(sizeof(MaxHeapNode) * config.post);
    if(trendPosts == NULL){
        printf("Failed to allocate trend post array.\n");
        fclose(postFile);
        FreeHashTagHashTable(hashTagTable);
        FreeUserHashTable(userTable);
        FreeGraph(graph);

        return 1;
    }

    long long trendPostID, postUserID, createdTime, likes;
    char title[50], content[50];
    int postCount = 0;

    while(fscanf(postFile, "%lld %lld %49s %49s %lld %lld", &trendPostID, &postUserID, title, content, &createdTime, &likes) == 6){
        trendPosts[postCount].postID = trendPostID;
        trendPosts[postCount].createdTime = createdTime;
        trendPosts[postCount].likes = likes;
        trendPosts[postCount].score = 0;
        postCount++;
    }

    fclose(postFile);

    // ======== Friend Ranking 생성 + Merge Sort ===========
    UserRanking* ranking = CreateFriendRanking(graph);
    if(ranking == NULL){
        printf("Failed to create friend ranking.\n");
        free(trendPosts);
        FreeHashTagHashTable(hashTagTable);
        FreeUserHashTable(userTable);
        FreeGraph(graph);
        return 1;
    }
    // 친구 수 기준 내림차순 병합 정렬
    MergeSortRanking(ranking, 0, graph->userCount - 1);

    // ======================= 메뉴 ========================
    int running = 1;
    int menu;

    while(running){
        printf("\n==================== MENU ====================\n");
        printf("1. Search User Profile\n");
        printf("2. Search Post by HashTag\n");
        printf("3. Find Shortest Relationship\n");
        printf("4. Recommend Friends\n");
        printf("5. Show Trend Feed\n");
        printf("6. Show Friend Ranking\n");
        printf("7. Exit\n");
        printf("Select Menu : ");

        if(scanf("%d", &menu) != 1){
            printf("Invaild Input.\n");
            // 입력 버퍼 날리기
            int ch;
            while((ch = getchar()) != '\n' && ch != EOF){
            }
            continue;
        }

        switch (menu)
        {
        case 1: // Search user
            long long searchUserID;
            printf("Enter User ID : ");
            if(scanf("%lld", &searchUserID) != 1){
                printf("Invaild Input.\n");
                // 입력 버퍼 날리기
                int ch;
                while((ch=getchar()) != '\n' && ch != EOF){};
                break;
            }
            // 유저 프로필 탐색
            UserProfile* resultProfile = SearchProfile(userTable, searchUserID);
            if(resultProfile == NULL){
                printf("User not found.\n");
            }else {
                printf("\n==================== USER PROFILE ===================\n");
                printf("User ID : %lld\n", resultProfile->userID);
                printf("Age     : %d\n", resultProfile->age);
                printf("Region  : %s\n", resultProfile->region);
                printf("Interest: %s\n", resultProfile->interest);
                printf("Status  : %s\n", resultProfile->status);
            }
            break;
        
        case 2: // Search HashTag
            char searchHashTag[50];
            printf("Available HashTags : HASHTAG001 ~ HASHTAG100\n");
            printf("Enter HashTag : ");
            if(scanf("%49s", searchHashTag) != 1){
                printf("Invaild Input.\n");
                break;
            }
            // 해시태그 탐색
            HashTagHashNode* resultHashTag = SearchHashTag(hashTagTable, searchHashTag);
            if(resultHashTag == NULL){
                printf("HashTag not found.\n");
            }else {
                printf("\n==================== POSTS : %s ====================\n", searchHashTag);
                // 하나의 해시태그에 연결된 게시물 목록
                PostIDNode* currentHashTag = resultHashTag->postList;
                int countPost = 0;
                // 인접리스트 순회
                while(currentHashTag != NULL){
                    printf("Post ID : %lld\n", currentHashTag->postID);
                    countPost++;
                    currentHashTag = currentHashTag->next;
                }
                printf("Total Posts : %d\n", countPost);
            }
            break;

        case 3: // Dijkstra
            int startUser, endUser;
            printf("Start User ID : ");
            if(scanf("%d", &startUser) != 1){
                printf("Invaild Input.\n");
                // 입력 버퍼 날리기
                int ch;
                while((ch = getchar()) != '\n' && ch != EOF){}
                break;
            }
            printf("End User ID : ");
            if(scanf("%d", &endUser) != 1){
                printf("Invaild Input.\n");
                // 입력 버퍼 날리기
                int ch;
                while((ch = getchar()) != '\n' && ch != EOF){}
                break;
            }

            long long* distance = (long long*)malloc(sizeof(long long) * (graph->userCount + 1));
            int* prev = (int*)malloc(sizeof(int) * (graph->userCount + 1));
            if(distance == NULL || prev == NULL){
                printf("Failed to allocate Dijkstra memory.\n");
                free(distance);
                free(prev);
                break;
            }

            if(Dijkstra(graph, startUser, endUser, distance, prev) != 0){
                printf("Invaild User ID.\n");
            }else if(distance[endUser] == LLONG_MAX){
                printf("User %d and User %d are not connected.\n", startUser, endUser);
            }else {
                printf("Shortest Distance : %lld\n", distance[endUser]);
                PrintPath(prev, startUser, endUser, graph->userCount);
            }
            free(distance);
            free(prev);
            break;

        case 4: // 추천 친구
            int recommendUser;
            printf("Enter User ID :");
            if(scanf("%d", &recommendUser) != 1){
                printf("Invaild Input.\n");
                // 입력 버퍼 날리기
                int ch;
                while((ch = getchar()) != '\n' && ch != EOF){}
                break;
            }
            
            int recommendCount = RecommendFriends(graph, recommendUser);
            if(recommendCount < 0){
                printf("Invaild User ID.\n");
            }else if(recommendCount == 0){
                printf("No recommended friends.\n");
            }else {
                printf("Recommended Friend Count : %d\n", recommendCount);
            }
            break;

        case 5: // 트렌드 피드 Top 10 (최대힙)
            // 트렌드 피드를 실행할때 마다 새로운 최대 힙 생성
            MaxHeap* trendHeap = CreateMaxHeap(postCount);
            if(trendHeap == NULL){
                printf("Failed to create trend heap.\n");
                break;
            }

            // 실행 시점의 현재 시간 (이거로 할때마다 다른 값의 최대힙 생성)
            long long currentTime = (long long)time(NULL);
            int insertFailed = 0; // 게시물 삽입 실패시 불완전한 힙으로 출력 시도 방지용

            // 현재 시간을 기준으로 모든 게시물 점수 재계산
            for(int i = 0; i < postCount; i++){
                long long score = CalculateTrendScore(trendPosts[i].createdTime, trendPosts[i].likes, currentTime);

                if(InsertMaxHeap(trendHeap, trendPosts[i].postID, trendPosts[i].createdTime, trendPosts[i].likes, score) != 0){
                    printf("Failed to insert post into trend heap.\n");
                    insertFailed = 1;
                    break;
                }
            }
            // 삽입 실패시 출력하지 않고 case 5 종료
            if(insertFailed){
                FreeMaxHeap(trendHeap);
                break;
            }
            printf("\n==================== TREND TOP 10 ====================\n");
            // 최대 힙에서 가장 높은 게시물 부터 10개 추출
            for(int i = 1; i <= 10; i++){
                MaxHeapNode result;

                if(ExtractMax(trendHeap, &result) != 0){
                    break;
                }
                printf("%d. Post %lld | Score : %lld | Likes : %lld\n", i, result.postID, result.score, result.likes);
            }
            // 다음 트렌드 피드 실행시 새로운 힙을 생성하므로 현재 힙 해제
            FreeMaxHeap(trendHeap);
            break;
        
        case 6: // 친구 수 top 50 출력
            PrintTopRanking(ranking);
            break;
        
        case 7: // 프로그램 종료
            running = 0;
            break;
            
        default:
            printf("Invaild Menu.\n");
            break;
        }
    }

    // ==================== 메모리 해제 ====================
    free(ranking);
    free(trendPosts);
    FreeHashTagHashTable(hashTagTable);
    FreeUserHashTable(userTable);
    FreeGraph(graph);
    // 최대 힙은 case 5 안에서 할당해제함

    return 0;
}




// 컴파일 코드 : gcc -g -O2 src/main.c src/graph.c src/generatorUtil.c src/hashTable.c src/minHeap.c src/dijkstra.c src/recommend.c src/maxHeap.c src/ranking.c -Iinclude -o compile/main
// valgrind 코드 : valgrind --leak-check=full --show-leak-kinds=all ./compile/main

