#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // LLONG_MAX 

#include "dijkstra.h"
#include "minHeap.h"

// 탐색할 유저 그래프, 시작 유저, 목적지 유저, 각 유저까지의 최단 거리 저장, 최단 경로에서 이전 유저 저장(부모 노드)
int Dijkstra(Graph* graph, int startUser, int endUser, long long* distance, int* prev){
    if(graph == NULL || distance == NULL || prev == NULL){
        return 1;
    }
    
    // 시작/목적지 유저가 1이상이어야함 + 시작/목적지 유저는 유저 최대치보다 이하여야함
    if(startUser < 1 || startUser > graph->userCount || endUser < 1 || endUser > graph->userCount){
        return 1;
    }

    // 모든 유저의 거리를 최대치로 초기화
    for(int i=1; i<=graph->userCount; i++){
        distance[i] = LLONG_MAX;
        prev[i] = -1; // 얘는 저장 안됌 상태를 의미 (부모 노드 인덱스 = userID)
    }

    distance[startUser] = 0; // 시작 유저의 자신까지의 거리 = 0

    MinHeap* heap = CreateMinHeap(graph->userCount); // capacity = userCount
    if(heap == NULL){
        return 1;
    }
    // 시작 유저 삽입
    if(InsertMinHeap(heap, startUser, 0) != 0){
        FreeMinHeap(heap);
        return 1;
    }

    // 다익스트라 본체
    while(heap->size > 0){
        // current에는 distance가 가장 작은 값이 들어감
        MinHeapNode current; 
        if(ExtractMin(heap, &current) != 0){ // break 안걸리면 남은 후보중 최단거리임
            break;
        }
        // 오래된 힙 데이터 검사 (중복 갱신 문제 해결용)
        if(current.distance != distance[current.userID]){
            continue;
        }
        // 목적지 도착
        if(current.userID == endUser){
            break;
        }

        // 현재 유저의 인접리스트 탐색
        ListNode* near = graph->list[current.userID];

        while(near != NULL){
            int nextUser = near->userID;

            long long newDistance = current.distance + 1; // 모든 간선 가중치는 1 
            // 기존 거리보다 더 짧은 경로 발견
            if(newDistance < distance[nextUser]){
                distance[nextUser] = newDistance;

                prev[nextUser] = current.userID; // 최단 경로 복원용 이전 노드 저장

                // 새로운 최단거리를 기준으로 최소 힙에 삽입
                if(InsertMinHeap(heap, nextUser, newDistance)!=0){
                    FreeMinHeap(heap);
                    return 1;
                }
            }
            near = near->next;
        }
    }
    FreeMinHeap(heap);
    return 0;
}

// 다익스트라 테스트용 코드
int main(){
    // 테스트 그래프 생성
    Graph* graph = CreateGraph(5);
    if(graph == NULL){
        printf("Failed to create graph.\n");
        return 1;
    }

    // 테스트용 간선
    AddEdge(graph, 1, 2);
    AddEdge(graph, 1, 3);
    AddEdge(graph, 2, 4);
    AddEdge(graph, 2, 5);
    AddEdge(graph, 3, 5);

    // distance 배열, prev 배열
    long long* distance = (long long*)malloc(sizeof(long long) * 6);
    int* prev = (int*)malloc(sizeof(int) * 6);
    if(distance == NULL || prev == NULL){
        printf("Failed to allocate memory.\n");

        free(distance);
        free(prev);
        FreeGraph(graph);

        return 1;
    }
    // 1 -> 4 최단 경로 찾기
    int startUser = 1;
    int endUser = 4;

    // Dijstra 실행
    if(Dijkstra(graph, startUser, endUser, distance, prev) != 0){
        printf("Dijstra failed.\n");
        free(distance);
        free(prev);
        FreeGraph(graph);
        return 1;
    }

    // 최단거리 출력
    if(distance[endUser] == LLONG_MAX){
        printf("User %d and User %d are not connected", startUser, endUser);
    } else {
        printf("Shortest distance from User %d to User %d : %lld\n", startUser, endUser, distance[endUser]);
    }

    // prev 배열 확인용
    printf("Previous nodes:\n");
    for(int i=1; i<=5; i++){
        printf("User %d : prev = %d, distance = %lld\n", i, prev[i], distance[i]);
    }

    free(distance);
    free(prev);
    FreeGraph(graph);

    return 0;
}