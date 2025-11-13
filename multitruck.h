#ifndef MULTITRUCK_H
#define MULTITRUCK_H

#include "truck.h"
#include "dijkstra.h"

typedef struct MultiTruckPlan {
    int truckID;
    int route[100];
    int routeLen;
    int totalDistance;
    int wasteCollected;
    float utilization;
} MultiTruckPlan;

void multiTruckDispatch(int n, int adj[][n], int start, bin binArr[], 
                       location loc[], truck trucks[], int numTrucks,
                       MultiTruckPlan plans[], int *numPlans);

void displayMultiTruckPlan(MultiTruckPlan plans[], int numPlans, location loc[]);

#endif