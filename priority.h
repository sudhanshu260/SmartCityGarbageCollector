#ifndef PRIORITY_H
#define PRIORITY_H

#include "dijkstra.h"

void priorityBasedCollection(int n, int adj[][n], int start, bin binArr[], 
                            location loc[], int route[], int *routeLen, 
                            int *totalDist, int truckCapacity);

int calculatePriority(int fillLevel, int distance, int basePriority);

#endif