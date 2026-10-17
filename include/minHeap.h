#ifndef MIN_HEAP_H
#define MIN_HEAP_H

// 다익스트라용 구조체
typedef struct {
    int userID;
    long long distance; 
} MinHeapNode;

// 최소 힙
typedef struct {
    int size;
    int capacity;
    MinHeapNode* nodes;
} MinHeap;

MinHeap* CreateMinHeap(int capacity);
// 최대힙이랑 비교 방향만 제외하면 거의 같음 (최소힙은 parent.distance <= child.distance)
int InsertMinHeap(MinHeap* heap, int userID, long long distance);
int ExtractMin(MinHeap* heap, MinHeapNode* result);
void FreeMinHeap(MinHeap* heap);
void SwapMinHeapNode(MinHeapNode* a, MinHeapNode* b);

#endif // MIN_HEAP_H