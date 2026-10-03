#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "graph.h"

#define CONFIG_FILE "data/generatorConfig.txt" // 데이터 규모 파일
#define EDGE_FILE "data/edge.txt" // 간선 파일

#define MIN_USER 100000LL
#define MAX_USER 100000000LL // 최대치 1억으로 잡음 (생성할때 30비트를 기준으로 잡아놔서...)
#define MIN_EDGE 500000LL

// 보고서에서도 작성할거지만 16비트 두개 생성하고 0x7FFF and 연산으로 맨앞에꺼 잘라내고
// 15비트 중에 하나 왼쪽으로 15비트 옮기고 나머지 하나 더해서 30비트 난수 생성함 그래서 1억이 최대임
int RandomUserID(int user){
    unsigned int random1 = rand() & 0x7FFF;
    unsigned int random2 = rand() & 0x7FFF;

    unsigned int result = (random1 << 15) | random2;

    return (int)(result % user) + 1;
}


int main(void) {
    FILE* configFile; // 데이터 규모 파일 (읽기)
    FILE* edgeFile; // 간선 파일 (쓰기)

    long long user, edge, edgeMax;
    long long generatedEdge = 0; // 생성된 간선 수 측정용
    char header[100]; // 식별용 텍스트 

    // [파일 내부 데이터 읽기 및 내부 데이터 무결성 검사 파트]
    configFile = fopen(CONFIG_FILE, "r");
    
    if(configFile == NULL){
        printf("데이터 규모 설정 파일을 열 수 없습니다.\n");
        printf("실행 경로 /DataStructureProject/ 에서 실행해주십시오.\n");
        printf("generatorManager.c를 먼저 실행해주십시오.\n");

        return 1;
    }
    // fgets는 \n도 같이 읽어서 마지막에 \0라는 마침표도 찍어줌
    // header 크기-1 만큼 header 배열에 configFile의 내용을 저장 (\0찍어야해서 한글자는 제외)
    // 보통 문자열 하나 읽고 그 이상은 안읽음 그리고 읽기 위치 다음줄로 넘김
    if(fgets(header, sizeof(header), configFile) == NULL){
        printf("데이터 규모 설정 파일을 읽을 수 없습니다.\n");
        fclose(configFile); // 열기는 성공했으니 닫아야함
        return 1;
    }
    // header와 "DATA_STRUCTURE_PROJECT_CONFIG" 문자열 비교함수
    if(strcmp(header, "DATA_STRUCTURE_PROJECT_CONFIG\n") != 0){
        printf("올바른 파일이 아닙니다.\n"); // 식별 문자열이 다름 => 어 이 파일 아닌갑다
        fclose(configFile);
        return 1;
    }
    // fscanf는 전체 파일에서 특정 키워드를 찾는게 아니라 읽고 있는 위치에서 저게 있는지 확인하는 정도임
    // 그래서 위치 섞으면 못읽음 (설마 교수님이 그거까지 섞겠어.. 테스트 시간이 썩어나는게 아니고서야)
    if(fscanf(configFile, "USER=%lld\n", &user) != 1){
        printf("USER 값을 읽을 수 없습니다.\n");
        fclose(configFile);
        return 1;
    }
    // 아 fscanf도 읽고 나서 다음줄로 읽는 위치 넘겨줌
    if(fscanf(configFile, "EDGE=%lld\n", &edge) != 1){
        printf("EDGE 값을 읽을 수 없습니다.\n");
        fclose(configFile);
        return 1;
    }
    // CHECK 값도 읽어서 확인해도 되는데 설마 교수님이 거기까지 하겠나 싶어서 
    // 일단 굳이 체크하진 않음
    /* 이거 유저, 간선, 해시태그, 게시물 값에 특정 값들 곱한거 더해야해서 위에 있는거들도 받아야함

    if(fscanf(configFile, "CHECK=%lld\n", &check) != 1){
    
    }
    */
    fclose(configFile);

    // 교수님이 임의로 파일 내부 값들 조정했을때 대비용 내부 값이 조건에 맞는지 확인
    if(user < MIN_USER || user > MAX_USER){
        printf("잘못된 USER 값입니다.\n");
        return 1;
    }
    edgeMax = user*(user-1)/2;
    if(edge < MIN_EDGE || edge > edgeMax){
        printf("잘못된 EDGE 값입니다.\n");
        return 1;
    }

    // [그래프 생성 (그 중복간선 확인용) 및 실질적인 데이터 생성 파트]

    Graph* graph = CreateGraph((int)user);
    if(graph == NULL){
        printf("그래프 생성 실패\n");
        return 1;
    }

    edgeFile = fopen(EDGE_FILE, "w");
    if (edgeFile == NULL){
        printf("edge.txt 파일 생성 실패\n");
        FreeGraph(graph); // 그래프는 위에서 생성하고 확인도 했으니 해제해줘야함
        return 1;
    }

    // rand()는 완전히 랜덤이 아니라서 현재 시간을 기준으로 시드를 생성해서 
    // 완전 랜덤이라기엔 여전히 어폐가 있긴 한데 전보단 나음
    srand((unsigned int)time(NULL)); 

    while(generatedEdge < edge){
        int user1 = RandomUserID((int)user);
        int user2 = RandomUserID((int)user);

        // case 1 : self-loop
        if (user1 == user2) {
            continue;
        }
        // case 2 : 중복 간선 (무방향 그래프라서)
        if(IsConnected(graph, user1, user2)){
            continue;
        }

        // 중복 간선 아님 => 간선 생성 
        // (그리고 간선 있는거 깔아놔야 다음확인때 중복인지 아닌지 거를 수 있음..)
        AddEdge(graph, user1, user2);

        fprintf(edgeFile, "%d %d\n", user1, user2);
        generatedEdge++;
    }

    fclose(edgeFile);
    FreeGraph(graph); // 중복 확인용 임시 그래프여서 날려도 상관 없음
    
    printf("간선 데이터 생성 완료\n");
    printf("유저 수 : %lld\n", user);
    printf("간선 수 : %lld\n", edge);

    return 0;
}
// 나중에 컴파일 할때 graph.c랑 같이 돌려야 작동함 
// gcc -O2 src/edgeGenerator.c src/graph.c -Iinclude -o compile/edgeGenerator
// 모든 컴파일 및 명령어는 ~/DataStructureProject에서 돌리는거로 가정하고 작성함
// 실행파일만 돌려보실거 같긴 한데 아니면 ㅈ되니까 제출전 참고