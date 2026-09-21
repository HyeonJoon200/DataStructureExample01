#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

ListNode* CreateListNode(int userID){ // 사실상 연결리스트 같은데..?
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

}

void AddEdge(Graph* graph, int user1, int user2){

}

void FreeGraph(Graph* graph){

}