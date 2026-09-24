#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

#define NODE 100000
#define EDGE 500000

int main(void) {
    // 코드 실행 시간 측정용 
    clock_t start = clock();

    srand((unsigned int)time(NULL));

    // 쓰기(write) 모드로 파일 열기 (안만들어 놨는데 뭐지)
    FILE *file = fopen("data/edge.txt", "w");
    if (file == NULL) {
        printf("파일 생성 실패 (edge.txt)\n");
        return 1;
    }
    Graph* graph = CreateGraph(NODE);
    if (graph == NULL){ // 실패시 처리
        return 1;
    }
    printf("데이터 생성 시작 (NODE: %d, EDGE: %d)\n", NODE, EDGE);

    int count = 0;
    while (count < EDGE) {
        int user1 = rand() % NODE + 1;
        int user2 = rand() % NODE + 1;
        
        // 지 스스로 연결되는거 방지용 코드
        if(user1 == user2) {
            continue;
        }
        // 이미 존재하는 간선이면 버리기
        if (IsConnected(graph, user1, user2)){ // 1이면 중복간선, 0이면 중복 아님
            continue;
        }
        // 중복 간선 아니면 그래프에 추가
        AddEdge(graph, user1, user2);
        // 저장 (안하면 ㅈ됨)
        fprintf(file, "%d %d\n", user1, user2);
        count++;

        if(count % 10000 == 0) {
            printf("%d개 간선 생성 완료\n", count);
        }
    }
    fclose(file);
    printf("데이터 생성 완료\n");

    // 코드 종료 시간 측정
    clock_t end = clock();

    // 실행시간 계산
    double time = (double)(end-start) / CLOCKS_PER_SEC;
    printf("실행시간 : %f s\n", time);
    printf("실행시간 : %f ms\n", time * 1000.0);
    
    return 0;
}
// 나중에 컴파일 할때 graph.c랑 같이 돌려야 작동함 
// gcc -O2 src/edgeGenerator.c src/graph.c -Iinclude -o compile/edgeGenerator
// 모든 컴파일 및 명령어는 ~/DataStructureProject에서 돌리는거로 가정하고 작성함
// 실행파일만 돌려보실거 같긴 한데 아니면 ㅈ되니까 제출전 참고