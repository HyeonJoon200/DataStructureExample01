#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "graph.h"
#include "generatorUtil.h"


#define EDGE_FILE "data/edge.txt" // 간선 파일

#define MIN_USER 100000LL
#define MAX_USER 100000000LL // 최대치 1억으로 잡음 (생성할때 30비트를 기준으로 잡아놔서...)
#define MIN_EDGE 500000LL

int GeneratorEdge(void) {
    FILE* edgeFile; // 간선 파일 (쓰기)

    long long edgeMax;
    long long generatedEdge = 0; // 생성된 간선 수 측정용
    
    GeneratorConfig config;
    if(LoadGeneratorConfig(&config) != 0){
        return 1;
    }

    long long user = config.user;
    long long edge = config.edge;

    // 교수님이 임의로 파일 내부 값들 조정했을때 대비용 내부 값이 조건에 맞는지 확인
    if(user < MIN_USER || user > MAX_USER){
        printf("Invaild USER value.\n");
        return 1;
    }
    edgeMax = user*(user-1)/2;
    if(edge < MIN_EDGE || edge > edgeMax){
        printf("Invaild EDGE value.\n");
        return 1;
    }

    // [그래프 생성 (그 중복간선 확인용) 및 실질적인 데이터 생성 파트]

    Graph* graph = CreateGraph((int)user);
    if(graph == NULL){
        printf("Failed to create graph\n");
        return 1;
    }

    edgeFile = fopen(EDGE_FILE, "w");
    if (edgeFile == NULL){
        printf("Failed to create edge.txt\n");
        FreeGraph(graph); // 그래프는 위에서 생성하고 확인도 했으니 해제해줘야함
        return 1;
    }

    while(generatedEdge < edge){
        int user1 = (int)RandomID(user) + 1;
        int user2 = (int)RandomID(user) + 1;

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
    
    printf("Edge data generation completed.\n");
    printf("User count : %lld\n", user);
    printf("Edge count : %lld\n", edge);

    return 0;
}
// 나중에 컴파일 할때 graph.c랑 같이 돌려야 작동함 
// gcc -O2 src/edgeGenerator.c src/graph.c -Iinclude -o compile/edgeGenerator
// 모든 컴파일 및 명령어는 ~/DataStructureProject에서 돌리는거로 가정하고 작성함
// 실행파일만 돌려보실거 같긴 한데 아니면 ㅈ되니까 제출전 참고