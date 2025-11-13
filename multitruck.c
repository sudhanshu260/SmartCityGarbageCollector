#include <stdio.h>
#include <stdlib.h>
#include "multitruck.h"

void multiTruckDispatch(int n, int adj[][n], int start, bin binArr[], 
                       location loc[], truck trucks[], int numTrucks,
                       MultiTruckPlan plans[], int *numPlans) {
    
    int visited[n];
    int remainingBins[n];
    int totalRemaining = 0;
    
    // Initialize arrays
    for(int i = 0; i < n; i++) {
        visited[i] = 0;
        remainingBins[i] = 0;
        if(binArr[i].hasBin && i != start) {
            remainingBins[i] = 1;
            totalRemaining++;
        }
    }
    
    printf("\n\n============================================================\n");
    printf("              MULTI-TRUCK DISPATCH SYSTEM\n");
    printf("============================================================\n");
    printf("Starting Depot: %s\n", loc[start].name);
    printf("Available Trucks: %d\n", numTrucks);
    printf("Total bins to collect: %d\n\n", totalRemaining);
    
    // Display truck fleet
    printf("TRUCK FLEET:\n");
    printf("------------\n");
    for(int i = 0; i < numTrucks; i++) {
        printf("Truck %d: Capacity %d units | Current Load: %d units\n", 
               i+1, trucks[i].capacity, trucks[i].currentLoad);
    }
    printf("\n");
    
    *numPlans = 0;
    int currentTruck = 0;
    int binsCollectedTotal = 0;
    
    while(totalRemaining > 0 && currentTruck < numTrucks) {
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("Dispatching Truck %d (%d units capacity)\n", 
               currentTruck + 1, trucks[currentTruck].capacity);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        
        MultiTruckPlan *plan = &plans[*numPlans];
        plan->truckID = currentTruck + 1;
        plan->routeLen = 0;
        plan->totalDistance = 0;
        plan->wasteCollected = 0;
        
        int currentPos = start;
        int currentLoad = 0;
        plan->route[plan->routeLen++] = currentPos;
        
        int step = 1;
        int binsCollectedThisTruck = 0;
        
        while(totalRemaining > 0 && currentLoad < trucks[currentTruck].capacity) {
            int dist[n], parent[n];
            dijkstra(n, adj, currentPos, dist, parent);
            
            // Find nearest unvisited bin that fits in remaining capacity
            int nextBin = -1;
            int minDist = __INT_MAX__;
            
            for(int i = 0; i < n; i++) {
                if(remainingBins[i] && !visited[i] && dist[i] < minDist) {
                    if(currentLoad + binArr[i].FullLevel <= trucks[currentTruck].capacity) {
                        minDist = dist[i];
                        nextBin = i;
                    }
                }
            }
            
            if(nextBin == -1) {
                // No more bins that can fit in current truck
                break;
            }
            
            // Collect this bin
            visited[nextBin] = 1;
            remainingBins[nextBin] = 0;
            plan->totalDistance += dist[nextBin];
            currentLoad += binArr[nextBin].FullLevel;
            plan->route[plan->routeLen++] = nextBin;
            binsCollectedThisTruck++;
            totalRemaining--;
            
            printf("Step %d: %s -> %s\n", step++, loc[currentPos].name, loc[nextBin].name);
            printf("        Distance: %d km | Bin Fill: %d units\n", 
                   dist[nextBin], binArr[nextBin].FullLevel);
            printf("        Truck Load: %d/%d units (%.1f%%)\n", 
                   currentLoad, trucks[currentTruck].capacity, 
                   (currentLoad * 100.0) / trucks[currentTruck].capacity);
            
            currentPos = nextBin;
        }
        
        // Return to depot
        int dist[n], parent[n];
        dijkstra(n, adj, currentPos, dist, parent);
        plan->totalDistance += dist[start];
        plan->route[plan->routeLen++] = start;
        
        plan->wasteCollected = currentLoad;
        plan->utilization = (currentLoad * 100.0) / trucks[currentTruck].capacity;
        
        printf("Return: %s -> %s (Distance: %d km)\n", 
               loc[currentPos].name, loc[start].name, dist[start]);
        printf("Truck %d Summary: %d bins, %d units, %.1f%% utilization\n\n",
               currentTruck + 1, binsCollectedThisTruck, currentLoad, plan->utilization);
        
        binsCollectedTotal += binsCollectedThisTruck;
        (*numPlans)++;
        currentTruck++;
    }
    
    printf("============================================================\n");
    printf("              MULTI-TRUCK DISPATCH COMPLETE\n");
    printf("============================================================\n");
    printf("Trucks Used: %d/%d\n", *numPlans, numTrucks);
    printf("Bins Collected: %d/%d\n", binsCollectedTotal, binsCollectedTotal + totalRemaining);
    printf("Remaining Bins: %d\n", totalRemaining);
    
    if(totalRemaining > 0) {
        printf("⚠️  Additional trucks needed for remaining bins!\n");
    } else {
        printf("✓ All bins collected successfully!\n");
    }
    printf("============================================================\n\n");
}

void displayMultiTruckPlan(MultiTruckPlan plans[], int numPlans, location loc[]) {
    printf("\n\n============================================================\n");
    printf("                 MULTI-TRUCK ROUTE PLANS\n");
    printf("============================================================\n");
    
    for(int i = 0; i < numPlans; i++) {
        printf("\n🚛 TRUCK %d ROUTE:\n", plans[i].truckID);
        printf("Route: ");
        for(int j = 0; j < plans[i].routeLen; j++) {
            printf("%s", loc[plans[i].route[j]].name);
            if(j < plans[i].routeLen - 1) printf(" → ");
        }
        printf("\nStats: Distance: %d km | Waste: %d units | Utilization: %.1f%%\n",
               plans[i].totalDistance, plans[i].wasteCollected, plans[i].utilization);
        printf("────────────────────────────────────────────────────────────\n");
    }
}