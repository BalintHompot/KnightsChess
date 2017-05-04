// heap.h is based on the version we used in course Algorithms and Data Structures


typedef struct State {
	int row;
	int column;
	int pathlen;
	int total;
} State;

typedef struct Heap {
	State *array;
	int front;
	int size;
} Heap;


Heap makeHeap () ;
State newState(int row, int column, int pathlen, int total);
int isEmptyHeap (Heap h) ;
void heapEmptyError() ;
void doubleHeapSize (Heap *hp) ;
void swap(State *pa, State *pb) ;
void enqueue (State n, Heap *hp) ;
void upheap(Heap *hp, int n);
void downheap (Heap *hp, int n) ;
State dequeue(Heap *hp) ;

