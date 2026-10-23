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

void PrintPath(int* prev, int startUser, int endUser, int userCount){
    int pathSize = 0;
    int* path = (int*)malloc(sizeof(int) * (userCount + 1));

    if(path == NULL){
        return;
    }

    int current = endUser;

    while(current != -1 && pathSize < userCount - 1){
        if(current < 1 || current > userCount){
            break;
        }
        
        path[pathSize] = current;
        pathSize++;

        if(current == startUser){
            break;
        }
        
        current = prev[current];
    }

    // startUser까지 도달 실패시
    if(pathSize == 0 || path[pathSize - 1] != startUser){
        free(path);
        return;
    }

    printf("Path : ");
    
    for(int i = pathSize - 1; i >= 0; i--){
        printf("%d", path[i]);
        if(i != 0){
            printf(" -> ");
        }
    }
    printf("\n");
    free(path);
}

