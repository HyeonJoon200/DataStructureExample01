#include <stdio.h> // 입출력하려면 필수
#include <stdlib.h> // 동적 할당용
#include <time.h> // 나중에 코드 실행시간 측정용으로 넣음
#include "graph.h"
#define NODE 100000 // 나중에 변동 가능하게 할거긴 한데 일단 테스트용으로 유저 수 고정함
#define EDGE 500000 // 위와 같음


// 모든 변수명은 camel 표기법으로 작성 (한단어면 그냥 소문자로 고정)
int main(){
    Graph* graph = CreateGraph(NODE);

    // 파일 열기 (읽기)
    FILE* file = fopen("data/edge.txt", "r");
    if(file == NULL) { // 파일 못 읽어오는거 대비용 예외처리 (어지간해서 그럴일 없을거 같긴 함...)
        printf("edge.txt 파일 읽기 실패\n");
        FreeGraph(graph); // 그래프 동적 할당 해제
        return 1; // 프로그램 중단
    }

    int user1, user2;

    // 한줄에서 정수 두개씩 읽는 동안은 반복되게
    while(fscanf(file, "%d %d", &user1, &user2) == 2) {
        // 읽어온 두 유저 사이의 간선 추가 (무방향임 AddEdge()에서 중복이면 알아서 처리함)
        AddEdge(graph, user1, user2);
    }

    fclose(file); // 파일 읽기 종료

    // 여기부터 그래프 기능 테스트 (9/29)
    // 일부 유저의 인접리스트 출력 테스트 (전체 유저로 하니까 터미널이 터져나가서 확인이 잘 안됌...)
    for(int i=1; i<=10; i++){ 
        printf("User %d : ", i);

        ListNode* current = graph->list[i];

        while(current != NULL){ // 연결리스트 순회용
            printf(" -> %d", current->userID);
            current = current->next;
        }
        printf("\n");
    }


    FreeGraph(graph);
    return 0;
}