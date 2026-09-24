#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

ListNode* CreateListNode(int userID){ // 사실상 연결리스트랑 구조 동일..
    ListNode* newUser = (ListNode*)malloc(sizeof(ListNode)); // 동적할당

    if (newUser == NULL) { // 동적할당 실패시 (설마 실패할까 싶긴한데 암튼)
        printf("메모리 할당 실패 CreateListNode\n");
        return NULL;
    }

    newUser->userID = userID; // 매개변수
    newUser->next = NULL; 

    return newUser;
}

Graph* CreateGraph(int nodeCount){
    Graph* graph = (Graph*)malloc(sizeof(Graph)); // 동적할당

    if (graph == NULL){
        printf("메모리 할당 실패 CreateGraph\n");
        return NULL;
    }

    graph->userCount = nodeCount;
    
    // 인접리스트 배열 할당
    graph->list = (ListNode**)malloc(sizeof(ListNode*)*(nodeCount+1)); // for문으로 반복해야하나
    if (graph->list == NULL){
        printf("메모리 할당 실패 CreateGraph List\n");
        free(graph);
        return NULL;
    }

    // 인접리스트 포인터 초기화 (근데 안되어있나...? 구조체 만들때 했던거 같은데)
    for (int i=0; i<=nodeCount; i++){
        graph->list[i] = NULL;
    }

    return graph;
}

int IsConnected(Graph* graph, int user1, int user2){ 
    // user1의 인접리스트 헤드 노드부터 시작
    ListNode* current = graph->list[user1];

    // user1의 친구목록 순차탐색
    while (current != NULL){
        // user2가 이미 친구목록에 있으면 중복 간선 => 날려야함
        if(current->userID == user2){
            // 중간에 끊는용 평소엔 flag 같은 boolean 변수 넣어서 처리하는데
            // 이렇게 하는게 빨라서 씀
            return 1;
        }
        // 다음 인접 노드 가서 검사해야함
        current = current->next;
    }

    // 전체 탐색 했는데 없으면 user2 연결 안된거 (중복 간선 아님)
    return 0;
}

void AddEdge(Graph* graph, int user1, int user2){
    ListNode* newUser1 = CreateListNode(user1); // user2의 친구목록에 user1을 추가하기 위한 노드
    ListNode* newUser2 = CreateListNode(user2); // user1의 친구목록에 user2를 추가하기 위한 노드

    if (newUser1 == NULL || newUser2 == NULL){ // 둘 중 하나라도 동적 할당 실패시 둘다 free하고 종료
        free(newUser1);
        free(newUser2);
        printf("메모리 할당 실패 AddEdge\n");
        return;
    }

    // user2의 기존 친구목록의 맨 앞(head)에 user1 추가
    newUser1->next = graph->list[user2];
    graph->list[user2] = newUser1;

    // user1의 기존 친구 목록의 맨 앞(head)에 user2 추가
    newUser2->next = graph->list[user1];
    graph->list[user1] = newUser2;
    // ㄴ 다만 이러면 문제가 한번에 두개의 간선을 깔아버려서 중복간선임 => IsConnected에서 한번만 날리면 되게끔... 
}
void FreeGraph(Graph* graph){
    // 그래프가 없는경우
    if (graph == NULL){
        return;
    }

    // 각 유저의 인접리스트 순회
    for(int i=0; i<= graph->userCount; i++){
        ListNode* current = graph->list[i];

        while (current != NULL){

            // free하기 전 다음 노드 위치 저장 (닐리기 전에 저장해놔야함) (순서 중요)
            ListNode* next = current->next;
            free(current); // 현재꺼 날리기 (free)
            current = next; // 다음 노드 저장해놨던거로 이동
        }
    }
    free(graph->list); // 인접리스트 시작 포인터 배열 해제
    free(graph); // Graph 자체 해제
}