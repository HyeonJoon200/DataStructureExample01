#ifndef MIN_HEAP_H
#define MIN_HEAP_H

typedef struct {
    int userID;
    long long distance; 
} MinHeapNode;

typedef struct {
    int size;
    int capacity;
    MinHeapNode* nodes;
} MinHeap;

MinHeap* CreateMinHeap(int capacity);
int InsertMinHeap(MinHeap* heap, int userID, long long distance);
int ExtractMin(MinHeap* heap, MinHeapNode* result);
void FreeMinHeap(MinHeap* heap);
void SwapMinHeapNode(MinHeapNode* a, MinHeapNode* b);

#endif // MIN_HEAP_H