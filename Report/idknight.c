#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "heap.h"
#include <string.h>

#define N 500   /* N times N chessboard */
double effectiveBranchingFactor(unsigned long states, int d) {
  /* approximates such that N=\Sum+{i=1}^{d} b^i */
  //printf("states %d depth %d\n", states, d);
  double lwb = 1.0;
  double div = (1.0/(double)d);
  //printf("div is %lf\n", div);
  double upb = pow(states,div );
  printf("%d\n", upb);
  while (upb - lwb > 0.000001) {
    double mid = (lwb + upb) / 2.0;
    /* the following test makes use of the geometric series */
    if (mid*(1-pow(mid, d))/(1-mid) <= states) {  
      lwb = mid;
    } else {
      upb = mid;
    }
  }
  return lwb;
}

int actions[8][2] = {  /* knight moves */
  {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1,-2}, {1,2}, {2, -1}, {2, 1}
};
int costShortestPath[N][N];
unsigned long statesVisited = 0;
unsigned long enqueued = 0;
unsigned long dequeued = 0;

int distanceFromGoal(int row, int col, int rowGoal, int colGoal){
	return (sqrt(pow((row-rowGoal), 2) + pow((col - colGoal), 2)))/sqrt(5);
}

int minimumSteps(int row, int col, int rowGoal, int colGoal){
	return (abs(rowGoal-row) + abs(colGoal-col))/3;
}

int heuristicFunction(int row, int col, int rowGoal, int colGoal, int heuristic){ //both functions return an integer value for simplicity reasons, since the decimal part of the heuristic value would not make a significant difference
	switch(heuristic){
		case 1:
			return distanceFromGoal(row, col, rowGoal, colGoal);
		case 2:
			return minimumSteps(row, col, rowGoal, colGoal);
	}
}

int isValidLocation(int x, int y) {
  return (0<=x && x < N && 0<= y && y < N);
}

void initialize() {
  int r, c;
  for (r=0; r < N; r++) {
    for (c=0; c < N; c++) {
      costShortestPath[r][c] = 999999; /* represents infinity */
    }
  }
}

int heurEval(char *heur){
	if(!strcmp(heur, "Distance")){return 1;}
	if(!strcmp(heur, "MinSteps")){return 2;}
	if(!strcmp(heur, "Both")){return 3;}
	return 0;
}

int isGoal(int row, int column, int rowGoal, int columnGoal){
	return rowGoal==row && columnGoal == column;
}

int knightDLS(int cost, int limit, int row, int column, int rowGoal, int columnGoal) {
  int act;
  statesVisited++;
  if (row == rowGoal && column == columnGoal) {
    return 1;  /* goal reached */
  }
  if (cost == limit || cost >= costShortestPath[row][column]) {
    return 0; /* limit reached, or we've been here before via a 'cheaper' path */
  }
  costShortestPath[row][column] = cost;
  for (act=0; act < 8; act++) {
    int r = row + actions[act][0];
    int c = column + actions[act][1];
    if (isValidLocation(r, c) && knightDLS(cost+1, limit, r, c, rowGoal, columnGoal) == 1) {
      return 1;
    }
  }
  return 0;
}

int knightIDS(int row, int column, int rowGoal, int columnGoal) {
	printf("---------starting iterative deepening search---------\n");
  int limit = 0;
  printf ("limit=0"); fflush(stdout);
  initialize();
  while (knightDLS(0, limit, row, column, rowGoal, columnGoal) == 0) {
    initialize();
    limit++;
    printf(",%d", limit); fflush(stdout);
  }
  printf("\n");
  return limit;
}

int knightAStar(int row, int column, int rowGoal, int columnGoal, int heuristic){
	printf("---------starting A* search---------\n---------heuristic is %d---------\n", heuristic);
	initialize();
	costShortestPath[row][column] = 0;
	Heap q = makeHeap();
	enqueue(newState(row, column, 0, heuristicFunction(row, column, rowGoal, columnGoal, heuristic)), &q);
	enqueued++;
	while (q.array!=NULL){
		State position = dequeue(&q);
		dequeued++;
		row = position.row;
		column = position.column;
		if (isGoal(row, column, rowGoal, columnGoal)){
			return position.pathlen;
		}
		//printf("position is %d %d goal is %d %d\n", row, column, rowGoal, columnGoal);
		for (int act=0; act < 8; act++) {
			int r = row + actions[act][0];
			int c = column + actions[act][1];
			if (isValidLocation(r, c)){
				//if (isGoal(r, c, rowGoal, columnGoal)){
					//return position.pathlen + 1;
				//}
				int estimatedPath = position.pathlen + 1 + heuristicFunction(r, c, rowGoal, columnGoal, heuristic);
				if(estimatedPath < costShortestPath[r][c]){
					enqueue(newState(r, c, position.pathlen + 1, estimatedPath), &q);
					costShortestPath[r][c] = estimatedPath;
					enqueued++;
				}
				
			}
		}
	}
	return 0;
}
void IDS(int x0, int y0, int x1, int y1){
	printf("Length shortest path: %d\n", knightIDS(x0,y0, x1,y1));
	printf("#visited states: %lu\n", statesVisited);
}
void AStar(int x0, int y0, int x1, int y1, int heuristic){
	if (heuristic < 3){
		int solution = knightAStar(x0,y0, x1,y1, heuristic);
		printf("Length shortest path: %d\n", solution);
		printf("#enqued states: %lu\n", enqueued);
		printf("#dequed states: %lu\n", dequeued);
		//printf("branching factor is %d\n", effectiveBranchingFactor(enqueued, solution));
	}else{
		for (int h=1; h<3; h++){
			enqueued=0;
			dequeued=0;
			int solution = knightAStar(x0,y0, x1,y1, h);
			printf("Length shortest path: %d\n", solution);
			printf("#enqued states: %lu\n", enqueued);
			printf("#dequed states: %lu\n", dequeued);
			//printf("branching factor is %d\n", effectiveBranchingFactor(enqueued, solution));
		}
	}
}
int main(int argc, char *argv[]) {
  int x0,y0, x1,y1;
  char method[5];
  char heurS[10];
  do {
    printf("Start location (x,y) = "); fflush(stdout);
    scanf("%d %d", &x0, &y0);
  } while (!isValidLocation(x0,y0));
  do {
    printf("Goal location (x,y)  = "); fflush(stdout);
    scanf("%d %d", &x1, &y1);
  } while (!isValidLocation(x1,y1));
	do {
	printf("Give a valid method (IDS|AStar|Both)\n");
	scanf("%s", method);
	}while(strcmp(method, "IDS")&&strcmp(method, "AStar")&&strcmp(method, "Both"));
	
	int heuristic;
	do {
	printf("Give a valid heuristic method (Distance|MinSteps|Both)\nDistance calculates the straight line distance to the goal, MinSteps calculates the minimum number of steps to the goal\n");
	scanf("%s", heurS);
	heuristic = heurEval(heurS);
	}while(heuristic == 0);
	
	if (!strcmp(method, "IDS")){
		IDS(x0,y0,x1,y1);
	}else if(!strcmp(method, "AStar")){
		AStar(x0, y0, x1, y1, heuristic);
	}else{
		IDS(x0,y0,x1,y1);
		printf("\n");
		AStar(x0, y0, x1, y1, heuristic);
	}
  return 0;
}
