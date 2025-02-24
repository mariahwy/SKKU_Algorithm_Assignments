#include <stdio.h>

#define MAX_SIZE 500000

#define min(x, y) (x) < (y) ? (x) : (y)
#define max(x, y) (x) > (y) ? (x) : (y)
#define swap(x, y) {int tmp = *(x); *(x) = *(y); *(y) = tmp;}

/*
Heap
I referred to "Data Structures in C" by Yoon Seong-woo to implement the heap data structure.
*/
typedef struct _MinMaxHeap{
    int size;
    int heap_arr[MAX_SIZE/2 + 1];
} MinMaxHeap;

typedef struct _PriorityQueue{
    MinMaxHeap small_heap;
    MinMaxHeap large_heap;
} PriorityQueue;

void Initialize_Heap(MinMaxHeap *heap){
    heap -> size = 0;
}

void Initialize_Priority_Queue(PriorityQueue *queue){
    Initialize_Heap(&queue -> small_heap);
    Initialize_Heap(&queue -> large_heap);
}

int GetParentIDX(int idx){
    return idx/2;
}

int GetLeftChildIDX(int idx){
    return idx*2;
}

int GetRightChildIDX(int idx){
    return idx*2+1;
}

// min : even , max : odd
int GetLevel(int idx){
    int level = 0;
    while (idx > 1){
        idx /= 2;
        level++;
    }
    return level % 2 == 0; // True: even -> Min level, False: odd -> max level 
}

/*
PushUp and PushDown for MinMax Heap
I referred to wikipedia to implement the MinMax heap.
https://en.wikipedia.org/wiki/Min-max_heap
*/
void PushUp(MinMaxHeap *heap, int idx){
    int parentIdx = GetParentIDX(idx);
    int grandparentIdx;

    if (GetLevel(idx)){ // min-level (parent)
        // 1. if heap_arr[idx] > parent : swap
        if (heap -> heap_arr[idx] > heap -> heap_arr[parentIdx]){
            swap(&heap -> heap_arr[idx], &heap -> heap_arr[parentIdx]);
            idx = parentIdx;
        }
        // 2. if idx is in min-level and heap_arr[idx] < grand_parent
        if (GetLevel(idx)){ // min-level (grandparent)
            // if heap_arr[idx] < grandparent : swap
            while (idx > 3) {
                grandparentIdx = GetParentIDX(GetParentIDX(idx));
                if (heap -> heap_arr[idx] < heap -> heap_arr[grandparentIdx]){
                    swap(&heap -> heap_arr[idx], &heap -> heap_arr[grandparentIdx]);
                    idx = grandparentIdx;
                } else break;
            }
        }
        else { // max-level (grandparent)
            while (idx > 3) {
                // if child > grandparent => swap
                grandparentIdx = GetParentIDX(GetParentIDX(idx));
                if (heap -> heap_arr[idx] > heap -> heap_arr[grandparentIdx]) {
                    swap(&heap -> heap_arr[idx], &heap -> heap_arr[grandparentIdx]);
                    idx = grandparentIdx;
                } else break;
            }
        }
    }
    else { // max-level (parent)
        // 1. if heap_arr[idx] < parent : swap
        if (heap -> heap_arr[idx] < heap -> heap_arr[parentIdx]){
            swap(&heap -> heap_arr[idx], &heap -> heap_arr[parentIdx]);
            idx = parentIdx;
        }
        // 2. if idx is in min-level and heap_arr[idx] < grand_parent
        if (GetLevel(idx)){ // min-level (grandparent)
            // if heap_arr[idx] < grandparent : swap
            while (idx > 3) {
                grandparentIdx = GetParentIDX(GetParentIDX(idx));
                if (heap -> heap_arr[idx] < heap -> heap_arr[grandparentIdx]){
                    swap(&heap -> heap_arr[idx], &heap -> heap_arr[grandparentIdx]);
                    idx = grandparentIdx;
                } else break;
            }
        }
        else { // max-level (grandparent)
            while (idx > 3) {
                // if child > grandparent => swap
                grandparentIdx = GetParentIDX(GetParentIDX(idx));
                if (heap -> heap_arr[idx] > heap -> heap_arr[grandparentIdx]) {
                    swap(&heap -> heap_arr[idx], &heap -> heap_arr[grandparentIdx]);
                    idx = grandparentIdx;
                } else break;
            }
        }
    }
}

int Min_Idx(MinMaxHeap *heap, int idx){
    int leftchildIdx, rightchildIdx;
    int grandchildIdx_1 , grandchildIdx_2, grandchildIdx_3, grandchildIdx_4;
    int minIdx;

    leftchildIdx = GetLeftChildIDX(idx);
    rightchildIdx = GetRightChildIDX(idx);
    grandchildIdx_1 = GetLeftChildIDX(GetLeftChildIDX(idx));
    grandchildIdx_2 = GetRightChildIDX(GetLeftChildIDX(idx));
    grandchildIdx_3 = GetLeftChildIDX(GetRightChildIDX(idx));
    grandchildIdx_4 = GetRightChildIDX(GetRightChildIDX(idx));

    minIdx = min(heap -> heap_arr[leftchildIdx], heap -> heap_arr[rightchildIdx]);
    minIdx = min(heap -> heap_arr[minIdx], heap -> heap_arr[grandchildIdx_1]);
    minIdx = min(heap -> heap_arr[minIdx], heap -> heap_arr[grandchildIdx_2]);
    minIdx = min(heap -> heap_arr[minIdx], heap -> heap_arr[grandchildIdx_3]);
    minIdx = min(heap -> heap_arr[minIdx], heap -> heap_arr[grandchildIdx_4]);

    return minIdx;
}

int Max_Idx(MinMaxHeap *heap, int idx){
    int leftchildIdx, rightchildIdx;
    int grandchildIdx_1 , grandchildIdx_2, grandchildIdx_3, grandchildIdx_4;
    int maxIdx;

    leftchildIdx = GetLeftChildIDX(idx);
    rightchildIdx = GetRightChildIDX(idx);
    grandchildIdx_1 = GetLeftChildIDX(GetLeftChildIDX(idx));
    grandchildIdx_2 = GetRightChildIDX(GetLeftChildIDX(idx));
    grandchildIdx_3 = GetLeftChildIDX(GetRightChildIDX(idx));
    grandchildIdx_4 = GetRightChildIDX(GetRightChildIDX(idx));

    maxIdx = max(heap -> heap_arr[leftchildIdx], heap -> heap_arr[rightchildIdx]);
    maxIdx = max(heap -> heap_arr[maxIdx], heap -> heap_arr[grandchildIdx_1]);
    maxIdx = max(heap -> heap_arr[maxIdx], heap -> heap_arr[grandchildIdx_2]);
    maxIdx = max(heap -> heap_arr[maxIdx], heap -> heap_arr[grandchildIdx_3]);
    maxIdx = max(heap -> heap_arr[maxIdx], heap -> heap_arr[grandchildIdx_4]);

    return maxIdx;
}

void PushDown(MinMaxHeap *heap, int idx){
    int minIdx, maxIdx;

    if (GetLevel(idx)){ // min-level
        // 1. select minimum of child and grandchild
        while (GetLeftChildIDX(idx) <= heap -> size){
            minIdx = Min_Idx(heap, idx);

            // 2. if heap_arr[minIdx] < heap_arr[idx]
            if (heap -> heap_arr[minIdx] < heap -> heap_arr[idx]){
               // 2-1. if minIdx is child of idx
               if (minIdx == GetLeftChildIDX(idx) || minIdx == GetRightChildIDX(idx)){
                    swap(&heap -> heap_arr[idx], &heap -> heap_arr[minIdx]); 
                    break;
               }
               // 2-2. if minIdx is grandchild of idx
               else {
                // 2-2-1. swap
                swap(&heap -> heap_arr[idx], &heap -> heap_arr[minIdx]);

                // 2-2-2. if idx is bigger than a's parent, swap a with a's parent
                if (heap -> heap_arr[minIdx] > heap -> heap_arr[GetParentIDX(minIdx)]){
                    swap(&heap -> heap_arr[minIdx], &heap -> heap_arr[GetParentIDX(minIdx)]);
                }

                // 2-2-3. continue with updated idx
                idx = minIdx;
               }
            }
        }
    }
    else { // max-level
         // 1. select maximum of child and grandchild
        while (GetLeftChildIDX(idx) <= heap -> size){
            maxIdx = Max_Idx(heap, idx);

            // 2. if heap_arr[maxIdx] > heap_arr[idx]
            if (heap -> heap_arr[maxIdx] > heap -> heap_arr[idx]){
               // 2-1. if maxIdx is child of idx
               if (maxIdx == GetLeftChildIDX(idx) || maxIdx == GetRightChildIDX(idx)){
                    swap(&heap -> heap_arr[idx], &heap -> heap_arr[maxIdx]); 
                    break;
               }
               // 2-2. if maxIdx is grandchild of idx
               else {
                // 2-2-1. swap
                swap(&heap -> heap_arr[idx], &heap -> heap_arr[maxIdx]);

                // 2-2-2. if idx is smaller than a's parent, swap a with a's parent
                if (heap -> heap_arr[maxIdx] > heap -> heap_arr[GetParentIDX(maxIdx)]){
                    swap(&heap -> heap_arr[maxIdx], &heap -> heap_arr[GetParentIDX(maxIdx)]);
                }

                // 2-2-3. continue with updated idx
                idx = maxIdx;
               }
            }
        }       
    }
}

void insert_heap(MinMaxHeap *heap, int element) {
    int n = heap -> size;

    heap->size++;
    heap -> heap_arr[n] = element;
    PushUp(heap, n);
}

int delete_min_heap(MinMaxHeap *heap) {
    int n = heap -> size;
    int min = heap -> heap_arr[0];

    if (n == 1){
        return -1;
    }

    heap -> heap_arr[0] = heap -> heap_arr[n-1];
    heap->size--;
    PushDown(heap, 0);

    return min;
}

int delete_max_heap(MinMaxHeap *heap) {
    int n = heap -> size;
    int max;

    if (n == 0){
        return -1;
    }
    else if (n == 1){
        max = heap -> heap_arr[0];
        heap->size--;
        return max;
    }
    else if (n == 2){
        max = heap -> heap_arr[1];
        heap->size--;
        return max;
    }
    else {
        if (heap -> heap_arr[1] > heap -> heap_arr[2]){
            max = heap -> heap_arr[1];
            heap -> heap_arr[1] = heap -> heap_arr[n-1];
            heap->size--;
            PushDown(heap, 1);
        }
        else {
            max = heap -> heap_arr[2];
            heap -> heap_arr[2] = heap -> heap_arr[n-1];
            heap->size--;
            PushDown(heap, 2);
        }
    }
    return max;
}

int find_min_heap(MinMaxHeap *heap){
    int min = heap -> heap_arr[0];

    return min;
}

int find_max_heap(MinMaxHeap *heap){
    int n = heap -> size;
    int max;

    if (n == 0){
        return -1;
    }
    else if (n == 1){
        max = heap -> heap_arr[0];
        return max;
    }
    else if (n == 2){
        max = heap -> heap_arr[1];
        return max;
    }
    else {
        if (heap -> heap_arr[1] > heap -> heap_arr[2]){
            max = heap -> heap_arr[1];
        }
        else {
            max = heap -> heap_arr[2];
        }
    }
    return max;
}

/*
Priority Queue:
Insert, Delete, Find Operation
*/
void insert(PriorityQueue *queue, int element){
    int min, max;
    int smallIdx = queue -> small_heap.size - 1;
    int largeIdx = queue -> large_heap.size - 1;

    if (queue -> small_heap.size == 0 && queue -> large_heap.size == 0){
        queue -> large_heap.heap_arr[0] = element;
    }
    else{
        if (element >= find_min_heap(&queue -> large_heap)){
            insert_heap(&queue -> large_heap, element);
        }
        else {
            insert_heap(&queue -> small_heap, element);
        }
    }

    if (queue -> small_heap.size > queue -> large_heap.size){
        min = delete_min_heap(&queue -> small_heap);
        insert_heap(&queue -> large_heap, min);
    }
    if (queue -> large_heap.size > queue -> small_heap.size + 1){
        max = delete_max_heap(&queue -> large_heap);
        insert_heap(&queue -> small_heap, max);
    }
}

int delete_min(PriorityQueue *queue){
    int deleted_min, deleted_max, min;

    if (queue -> large_heap.size == 0 || queue -> small_heap.size == 0){
        return -1;
    }
    else{
        if (queue -> small_heap.size == 0){
            min = delete_min_heap(&queue -> large_heap);
        }
        else {
            min = delete_min_heap(&queue -> small_heap);
        }

        // maintain the size of heap
        if (queue -> small_heap.size > queue -> large_heap.size){
            deleted_min = delete_min_heap(&queue -> small_heap);
            insert_heap(&queue -> large_heap, deleted_min);
        }
        if (queue -> large_heap.size > queue -> small_heap.size + 1){
            deleted_max = delete_max_heap(&queue -> large_heap);
            insert_heap(&queue -> small_heap, deleted_max);
        }

        return min;
    }
}

int delete_max(PriorityQueue *queue){
    int deleted_min, deleted_max, max;

    if (queue -> large_heap.size == 0 || queue -> small_heap.size == 0){
        return -1;
    }
    else{
        max = delete_max_heap(&queue -> large_heap);
        
        // maintain the size of heap
        if (queue -> small_heap.size > queue -> large_heap.size){
            deleted_min = delete_min_heap(&queue -> small_heap);
            insert_heap(&queue -> large_heap, deleted_min);
        }
        if (queue -> large_heap.size > queue -> small_heap.size + 1){
            deleted_max = delete_max_heap(&queue -> large_heap);
            insert_heap(&queue -> small_heap, deleted_max);
        }

        return max;
    }
}

int delete_median(PriorityQueue *queue){
    int deleted_min, deleted_max, median;

    if (queue -> large_heap.size == 0 || queue -> small_heap.size == 0){
        return -1;
    }
    else {
        if (queue -> small_heap.size == queue -> large_heap.size){ // even, compare small_heap_max and large_heap_min
            median = min(find_max_heap(&queue -> small_heap), find_min_heap(&queue -> large_heap));
        }
        else { // odd, large heap_min
            median = find_min_heap(&queue -> large_heap);
        }

        if (queue -> small_heap.size > queue -> large_heap.size){
            deleted_min = delete_min_heap(&queue -> small_heap);
            insert_heap(&queue -> large_heap, deleted_min);
        }
        if (queue -> large_heap.size > queue -> small_heap.size + 1){
            deleted_max = delete_max_heap(&queue -> large_heap);
            insert_heap(&queue -> small_heap, deleted_max);
        }
    
        return median;
    }
}

int find_min(PriorityQueue *queue){
    int min;

    if (queue -> large_heap.size == 0 || queue -> small_heap.size == 0){
        return -1;
    }
    else{
        if (queue -> small_heap.size == 0){
            min = find_min_heap(&queue -> large_heap);
            return min;
        }
        else {
            min = find_min_heap(&queue -> small_heap);
            return min;
        }
    }
}

int find_max(PriorityQueue *queue){
    int max;

    if (queue -> large_heap.size == 0 || queue -> small_heap.size == 0){
        return -1;
    }
    else{
        max = delete_max_heap(&queue -> large_heap);
        return -1;
    }
}

int find_median(PriorityQueue *queue){
    int median;

    if (queue -> large_heap.size == 0 || queue -> small_heap.size == 0){
        return -1;
    }
    else {
        if (queue -> small_heap.size == queue -> large_heap.size){ // even, compare small_heap_max and large_heap_min
            median = min(find_max_heap(&queue -> small_heap), find_min_heap(&queue -> large_heap));
        }
        else { // odd, large heap_min
            median = find_min_heap(&queue -> large_heap);
        }
        return median;
    } 
}          

int main(){
    FILE *input, *output;

    input = fopen("pq.in", "r");
    output = fopen("pq.out", "w");

    int op_num, i, value;
    char op_type, target;

    PriorityQueue queue;
    Initialize_Priority_Queue(&queue);

    fscanf(input, "%d", &op_num);
    for (i=0; i<op_num; i++){
        fscanf(input, "%c %d", &op_type, &value);
        
        if (op_type == 'I'){
            insert(&queue, value);
        }
        else if (op_type == 'D'){
            target = (char) value;
            if (target == 'M'){
                delete_min(&queue);
            }
            else if (target == 'X'){
                delete_max(&queue);
            }
            else if (target == 'E'){
                delete_median(&queue);
            }
        }
        else if (op_type == 'F'){
            target = (char) value;
            
            if (target == 'M'){
                if (find_min(&queue) == -1){
                    fprintf(output, "NULL\n");
                }
                else {
                    fprintf(output, "%d\n", find_min(&queue));
                }
            }
            else if (target == 'X'){
                if (find_max(&queue) == -1){
                    fprintf(output, "NULL\n");
                }
                else {
                    fprintf(output, "%d\n", find_max(&queue));
                }
            }
            else if (target == 'E'){
                if (find_median(&queue) == -1){
                    fprintf(output, "NULL\n");
                }
                else {
                    fprintf(output, "%d\n", find_median(&queue));
                }
            }
        }
    }

    fclose(input);
    fclose(output);

    return 0;
}
