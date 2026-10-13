#include <stdio.h> // 입출력하려면 필수
#include <stdlib.h> // 동적 할당용
#include <time.h> // 나중에 코드 실행시간 측정용으로 넣음
#include "graph.h"
#include "hashTable.h"
#include "generatorUtil.h"

#define EDGE_FILE "data/edge.txt"
#define PROFILE_FILE "data/profiles.txt"
#define HASHTAG_FILE "data/hashTags.txt"

// 모든 변수명은 camel 표기법으로 작성 (한단어면 그냥 소문자로 고정)
int main(){
    GeneratorConfig config;
    if(LoadGeneratorConfig(&config) != 0){
        return 1;
    }

    Graph* graph = CreateGraph(config.user);

    // 파일 열기 (읽기)
    FILE* file = fopen("data/edge.txt", "r");
    if(file == NULL) { // 파일 못 읽어오는거 대비용 예외처리 (어지간해서 그럴일 없을거 같긴 함...)
        printf("Failed to read edge.txt\n");
        FreeGraph(graph); // 그래프 동적 할당 해제
        return 1; // 프로그램 중단
    }

    long long user1, user2;

    // 한줄에서 정수 두개씩 읽는 동안은 반복되게
    while(fscanf(file, "%lld %lld", &user1, &user2) == 2) {
        // 읽어온 두 유저 사이의 간선 추가 (무방향임 AddEdge()에서 중복이면 알아서 처리함)
        AddEdge(graph, user1, user2);
    }

    fclose(file); // 파일 읽기 종료

    // 여기부터 그래프 기능 테스트 (9/29)
    // 일부 유저의 인접리스트 출력 테스트 (전체 유저로 하니까 터미널이 터져나가서 확인이 잘 안됌...)
    for(int i=1; i<=config.user; i++){ 
        printf("User %d : ", i);

        ListNode* current = graph->list[i];

        while(current != NULL){ // 연결리스트 순회용
            printf(" -> %d", current->userID);
            current = current->next;
        }
        printf("\n");
    } // (9/29)
    FreeGraph(graph);

    // ================ 10/13 유저 해시테이블 용 코드 =============

    UserHashTable* userTable = CreateUserHashTable((int)(config.user/2)); // 임시로 테이블 크기는 유저수의 반절로 설정
    if(userTable == NULL){
        printf("Failed to create user hash table.\n");
        return 1;
    }

    // profiles.txt 열기
    FILE* profileFile = fopen(PROFILE_FILE, "r");
    if(profileFile == NULL){
        printf("Failed to read profile file. (%s)\n", PROFILE_FILE);
        FreeUserHashTable(userTable);
        return 1;
    }

    // userTable에 프로필 데이터 읽어서 삽입
    UserProfile profile;
    while(fscanf(profileFile, "%lld %d %19s %19s %19s\n", &profile.userID, &profile.age, profile.region, profile.interest, profile.status) == 5){
        // 삽입 실패
        if(InsertProfile(userTable, profile) != 0){
            printf("Failed to insert profile.\n");
            fclose(profileFile); // 파일 닫기
            FreeUserHashTable(userTable); // 할당 해제
            return 1;
        }
    }
    fclose(profileFile);
    
    // userTable에서 데이터 검색
    long long searchID;
    printf("Enter User ID : ");
    // scanf 깡으로 썼다가 반환값 관련으로 경고 떠서 (컴파일 중에)
    if(scanf("%lld", &searchID) != 1){
        printf("Invaild input.\n");
        FreeUserHashTable(userTable);
        FreeGraph(graph);
        return 1;
    }

    UserProfile* result = SearchProfile(userTable, searchID);

    if(result != NULL){
        printf("%lld %d %s %s %s\n", result->userID, result->age, result->region, result->interest, result->status);
    } else {
        printf("Failed to search User%lld.\n", searchID);
    }

    // 할당 해제
    FreeUserHashTable(userTable);

    //=========================== 해시태그 용 해시테이블 (10/13) ==============================
    
    // hashtag 파일 읽어와서 테이블에 넣기 (create + insert)
    FILE* hashTagFile = fopen(HASHTAG_FILE, "r");

    if(hashTagFile == NULL){
        printf("Failed to open file. (%s)\n", HASHTAG_FILE);
        return 1;
    }

    HashTagHashTable* hashTagTable = CreateHashTagHashTable(101);

    if(hashTagFile == NULL){
        printf("Failed to create hashtag hash table.\n");
        fclose(hashTagFile);
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
            FreeHashTagHashTable(hashTagTable);

            return 1;
        }
    }
    fclose(hashTagFile);

    // hashtag search 검색
    char searchTag[50];
    printf("Enter hashtag : ");
    // 여기도 반환값 검사 안하면 컴파일중에 경고창 올라옴
    if(scanf("%49s", searchTag) != 1){
        printf("Invaild input.\n");
        FreeHashTagHashTable(hashTagTable);
        return 1;
    }

    HashTagHashNode* resultHashTag = SearchHashTag(hashTagTable, searchTag);

    if(resultHashTag == NULL){
        printf("HashTag not found. (%s)\n", searchTag);
    }else{
        // 위에 있는 다른 구조체로 선언한 result랑 같이 취급되길래 뒤에 2붙임 하단 current2도 동일 사유
        printf("HashTag : %s\n", resultHashTag->hashTagName);
        printf("Post ID list : ");

        PostIDNode* currentHashTag = resultHashTag->postList;

        while(currentHashTag != NULL){
            printf("%lld ", currentHashTag->postID);
            currentHashTag = currentHashTag->next;
        }
        printf("\n");
    }
    return 0;
}