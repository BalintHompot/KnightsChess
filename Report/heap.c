// heap.c is based on the version we used in course Algorithms and Data Structures

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "heap.h"
#include <string.h>

State newState(int row, int column, int pathlen, int total){
	State s;
	s.row = row;
	s.column = column;
	s.pathlen = pathlen;
	s.total = total;
	return s;
}

Heap makeHeap () {
	Heap h;
	h.array = malloc(1*sizeof(State));
	assert(h.array != NULL);
	h.front = 1;
	h.size = 1;
	return h;
}

int isEmptyHeap (Heap h) {
	return (h.front == 1);
}

void heapEmptyError() {
	printf("heap empty\n");
	abort();
}

void doubleHeapSize (Heap *hp) {
	//printf("doubleing heap size\n");
	int newSize = 2 * hp->size;
	hp->array = realloc(hp->array, newSize * sizeof(State));
	assert(hp->array != NULL);
	hp->size = newSize;
}

void swap(State *pa, State *pb) {
	State h = *pa;
	*pa = *pb;
	*pb = h;
}

void enqueue (State n, Heap *hp) {
	//printf ("enqueueing position with %c\n", n.type);
	int fr = hp->front;
	if ( fr == hp->size ) {
		doubleHeapSize(hp);
	}
	hp->array[fr] = n;
	upheap(hp,fr);
	//printf("the first position is %d %d\n", hp->array[1].col, hp->array[1].row);
	hp->front++;
}

void upheap(Heap *hp, int n){
	//printf("upheap started\n");
	if (n<=1){return;}
	if ( hp->array[n/2].total>hp->array[n].total ) {
		swap(&(hp->array[n]),&(hp->array[n/2]));
		upheap(hp,n/2);
	}
}

void downheap (Heap *hp, int n) {
	//printf("downheap started\n");
	int fr = hp->front;
	int indexMax = n;
	if ( fr < 2*n+1 ) { /* node n is a leaf, so nothing to do */
		return;
	}
	if ( hp->array[n].total > hp->array[2*n].total ) {
		indexMax = 2*n;
	}
	if ( fr > 2*n+1 && hp->array[indexMax].total > hp->array[2*n+1].total ) {
		indexMax = 2*n+1;
	}
	if ( indexMax != n ) {
		swap(&(hp->array[n]),&(hp->array[indexMax]));
		downheap(hp,indexMax);
	}
}

State dequeue(Heap *hp) {
	State n;
	if ( isEmptyHeap(*hp) ) {
		heapEmptyError();
	}
	n = hp->array[1];
	hp->front--;
	hp->array[1] = hp->array[hp->front];
	downheap(hp,1);
return n;
}
