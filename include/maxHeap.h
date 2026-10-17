#ifndef MAX_HEAP_H
#define MAX_HEAP_H

// 트렌드 피드용 구조체
typedef struct {
    long long postID;
    long long createdTime;
    long long likes;
    long long score;
} MaxHeapNode;

// 최대 힙
typedef struct {
    int size;
    int capacity;
    MaxHeapNode* nodes;
} MaxHeap;

MaxHeap* CreateMaxHeap(int capacity);
// 전체적으로 최소힙이랑 비교 방향만 반대고 나머진 거의 같음 (최대힙은 parent.score >= child.score)
int InsertMaxHeap(MaxHeap* heap, long long postID, long long createdTime, long long like, long long score);
int ExtractMax(MaxHeap* heap, MaxHeapNode* result);
void SwapMaxHeapNode(MaxHeapNode* a, MaxHeapNode* b);
void FreeMaxHeap(MaxHeap* heap);
// 트렌드 피드 점수 계산
long long CalculateTrendScore(long long createdTime, long long likes, long long currentTime);

#endif // MAX_HEAP_H