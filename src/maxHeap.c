#include <stdio.h>
#include <stdlib.h>

#include "maxHeap.h"

MaxHeap* CreateMaxHeap(int capacity){
    // MaxHeap 구조체 동적 할당
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));

    if(heap == NULL){
        return NULL;
    }
    // 시작시엔 노드가 없으니 size = 0
    heap->size = 0;
    // 노드 저장 최대치 설정
    heap->capacity = capacity;
    // 힙 배열 동적 할당 
    heap->nodes = (MaxHeapNode*)malloc(sizeof(MaxHeapNode) * capacity);

    if(heap->nodes == NULL){
        free(heap);
        return NULL;
    }
    return heap;
}
int InsertMaxHeap(MaxHeap* heap, long long postID, long long createdTime, long long likes, long long score){
    if(heap == NULL){
        return 1;
    }

    // 힙이 꽉 찬경우
    if(heap->size >= heap->capacity){
        return 1;
    }

    // 새로운 노드를 배열 마지막 위치에 삽입
    int index = heap->size;
    heap->nodes[index].postID = postID;
    heap->nodes[index].createdTime = createdTime;
    heap->nodes[index].likes = likes;
    heap->nodes[index].score = score;
    heap->size++;

    // 부모 노드 보다 score가 크면 위로 올림
    while(index > 0){
        int parent = (index - 1) / 2;
        // 부모노드가 더 크거나 같으면 최대 힙 조건에 만족
        if(heap->nodes[parent].score > heap->nodes[index].score){
            break;
        } 

        // 점수가 같고 postID가 작으면 최대힙 조건 만족 + ID를 작은쪽이 우선순위임
        if(heap->nodes[parent].score == heap->nodes[index].score && heap->nodes[parent].postID < heap->nodes[index].postID){
            break;
        }

        // 자식이 더 크면 최대힙 조건에서 벗어나기 때문에 부모와 위치 교환
        SwapMaxHeapNode(&heap->nodes[index], &heap->nodes[parent]);

        // 현위치를 부모 노드 위치로 갱신
        index = parent;
    }
    return 0;
}
int ExtractMax(MaxHeap* heap, MaxHeapNode* result){
    // 힙이 비어있는 경우
    if(heap==NULL || heap->size == 0){
        return 1;
    }

    // 루트가 최대값 -> 결과에 저장
    *result = heap->nodes[0];

    // 마지막 노드를 루트로 이동시킴
    heap->nodes[0] = heap->nodes[heap->size - 1];

    // 노드 갯수 감소
    heap->size--;

    int index = 0;

    while(1){
        int left = index*2+1;
        int right = index*2+2;
        int maximum = index;

        // 왼쪽 자식이 있다 AND (왼쪽이 현재보다 크다 OR (왼쪽과 현재 점수가 같다 AND 왼쪽보다 현재 postID가 크다)  maximum = left로 갱신
        if(left < heap->size && (heap->nodes[left].score > heap->nodes[maximum].score || (heap->nodes[left].score == heap->nodes[maximum].score && heap->nodes[left].postID < heap->nodes[maximum].postID))){
            maximum = left;
        }
        // 오른쪽 자식이 있다 AND (오른쪽이 현재보다 크다 OR (오른쪽과 현재 점수가 같다 AND 오른쪽보다 현재 postID가 크다)  maximum = right로 갱신
        if(right < heap->size && (heap->nodes[right].score > heap->nodes[maximum].score || (heap->nodes[right].score == heap->nodes[maximum].score && heap->nodes[right].postID < heap->nodes[maximum].postID))){
            maximum = right;
        }

        // 현재 노드가 가장 큰 값 => 최대힙 조건 충족
        if(maximum == index){
            break;
        }
        // 더 큰 자식과 교환하기
        SwapMaxHeapNode(&heap->nodes[index], &heap->nodes[maximum]);
        index = maximum;
    }
    return 0;
}
void SwapMaxHeapNode(MaxHeapNode* a, MaxHeapNode* b){
    MaxHeapNode temp = *a;
    *a = *b;
    *b = temp;
}
void FreeMaxHeap(MaxHeap* heap){
    if(heap == NULL){
        return;
    }
    free(heap->nodes);
    free(heap);
}
long long CalculateTrendScore (long long createdTime, long long likes, long long currentTime){
    long long passedTime = currentTime - createdTime;
    int weight; // 시간 가중치 (1시간 이내 : x10, 24시간 이내 : x8, 7일 이내 : x6, 30일 이내 : x4, 30일 초과 : x2)
    
    if(passedTime <= 60*60){
        weight = 10;
    }else if(passedTime <= 24*60*60){
        weight = 8;
    }else if(passedTime <= 24*60*60*7LL){
        weight = 6;
    }else if(passedTime <= 24*60*60*30LL){
        weight = 4;
    }else {
        weight = 2;
    }

    return likes * weight;
}