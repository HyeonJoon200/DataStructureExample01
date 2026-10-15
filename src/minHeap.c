#include <stdio.h>
#include <stdlib.h>
#include "minHeap.h"

MinHeap* CreateMinHeap(int capacity){   // int size, int capacity, MinHeapNode* nodes
    // MinHeap 구조체 동적 할당
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));

    if(heap == NULL){
        return NULL;
    }

    // 시작할땐 노드가 없으니 size = 0
    heap->size = 0;
    // 노드 저장 최대치
    heap->capacity = capacity;
    // 배열 동적할당
    heap->nodes = (MinHeapNode*)malloc(sizeof(MinHeapNode)*capacity);

    if(heap->nodes == NULL){ 
        free(heap);
        return NULL;
    }
    return heap; // callee에서 동적할당한거 Caller로 반환해줌 그리고 그걸 FreeMinHeap로 보내서 해제함
}

void SwapMinHeapNode(MinHeapNode* a, MinHeapNode* b){
    MinHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

int InsertMinHeap(MinHeap* heap, int userID, long long distance){
    // 힙이 꽉찬경우
    if(heap->size >= heap->capacity){
        return 1; // 실패
    }

    // 새로운 노드를 배열 마지막 위치에 삽입
    int index = heap->size;
    heap->nodes[index].userID = userID;
    heap->nodes[index].distance = distance;
    heap->size++;

    // 부모노드 보다 distance 가 작으면 위로 올림
    while(index>0){
        int parent = (index - 1) / 2;
        // 부모가 더 작거나 같으면 최소 힙 조건에 만족
        if(heap->nodes[parent].distance <= heap->nodes[index].distance){
            break;
        }

        // 자식이 더 작으면 최소 힙 조건에 벗어나기 때문에 부모와 위치 교환
        SwapMinHeapNode(&heap->nodes[index], &heap->nodes[parent]);

        // 현위치를 부모 위치로 이동
        index = parent;
    }
    return 0; // 성공
}

int ExtractMin(MinHeap* heap, MinHeapNode* result){
    // 힙이 빈 경우
    if(heap == NULL || heap->size == 0){
        return 1; // 실패
    }

    // 루트가 최소값 -> 결과에 저장
    *result = heap->nodes[0];

    // 마지막 노드를 루트로 이동시킴
    heap->nodes[0] = heap->nodes[heap->size-1];

    // 노드 갯수 감소
    heap->size--;

    int index = 0;

    while(1){
        int left = index*2+1;
        int right = index*2+2;
        int smallest = index;

        // 왼쪽 자식이 있고 현재보다 작으면 smallest 갱신
        if(left < heap->size && heap->nodes[left].distance < heap->nodes[smallest].distance){
            smallest = left;
        }
        // 오른쪽도 있고 현재보다 작으면 갱신
        if(right < heap->size && heap->nodes[right].distance < heap->nodes[smallest].distance){
            smallest = right;
        }

        // 현재 노드가 가장 작은 값 => 힙 조건 충족
        if(smallest == index){
            break;
        }

        // 더 작은 자식과 교환하기
        SwapMinHeapNode(&heap->nodes[index], &heap->nodes[smallest]);

        index = smallest;
    }
    return 0; // 성공
}

void FreeMinHeap(MinHeap* heap){
    if(heap == NULL){
        return;
    }

    free(heap->nodes);
    free(heap);
}

// 다익스트라 구현전 테스트용 임시 main
int main(){
    MinHeap* heap = CreateMinHeap(10);
    if(heap==NULL){
        printf("Failed to create minHeap.\n");
        return 1;
    }

    InsertMinHeap(heap, 1, 10);
    InsertMinHeap(heap, 2, 3);
    InsertMinHeap(heap, 3, 7);
    InsertMinHeap(heap, 4, 1);
    InsertMinHeap(heap, 5, 5);

    MinHeapNode result;
    while(ExtractMin(heap, &result) == 0){
        printf("userID : %d, distance : %lld\n", result.userID, result.distance);
    }
    FreeMinHeap(heap);

    return 0;
}