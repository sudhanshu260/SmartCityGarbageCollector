#include <stdio.h>
#include <stdlib.h>
#include "priority.h"

int calculatePriority(int fillLevel, int distance, int basePriority) {
    // Higher fill level = higher priority
    // Shorter distance = higher priority
    // Base priority can be used for emergency bins, etc.
    return (fillLevel * 2) - distance + (basePriority * 10);
}

void priorityBasedCollection(int n, int adj[][n], int start, bin binArr[], 
                            location loc[], int route[], int *routeLen, 
                            int *totalDist, int truckCapacity) {
    
    int visited[n];
    int priorities[n];
    
    for(int i = 0; i < n; i++) {
        visited[i] = 0;
        priorities[i] = 0;
    }
    
    printf("\n\n============================================================\n");
    printf("           PRIORITY-BASED COLLECTION SYSTEM\n");
    printf("============================================================\n");
    printf("Starting from: %s\n", loc[start].name);
    printf("Truck Capacity: %d units\n\n", truckCapacity);
    
    // Calculate initial priorities for all bins
    int dist[n], parent[n];
    dijkstra(n, adj, start, dist, parent);
    
    printf("BIN PRIORITIES CALCULATION:\n");
    printf("---------------------------\n");
    for(int i = 0; i < n; i++) {
        if(binArr[i].hasBin && i != start) {
            priorities[i] = calculatePriority(binArr[i].FullLevel, dist[i], 0);
            printf("%s: Fill Level=%d%%, Distance=%dkm, Priority Score=%d\n",
                   loc[i].name, binArr[i].FullLevel, dist[i], priorities[i]);
        }
    }
    printf("\n");
    
    int currentPos = start;
    int currentLoad = 0;
    *routeLen = 0;
    *totalDist = 0;
    
    route[(*routeLen)++] = currentPos;
    
    int totalBins = 0;
    for(int i = 0; i < n; i++) {
        if(binArr[i].hasBin && i != start) {
            totalBins++;
        }
    }
    
    int binsCollected = 0;
    int step = 1;
    
    while(binsCollected < totalBins && currentLoad < truckCapacity) {
        // Recalculate distances from current position
        dijkstra(n, adj, currentPos, dist, parent);
        
        // Recalculate priorities considering current position
        for(int i = 0; i < n; i++) {
            if(binArr[i].hasBin && !visited[i] && i != currentPos) {
                priorities[i] = calculatePriority(binArr[i].FullLevel, dist[i], 0);
            }
        }
        
        // Find bin with highest priority that fits
        int nextBin = -1;
        int highestPriority = -1;
        
        for(int i = 0; i < n; i++) {
            if(binArr[i].hasBin && !visited[i] && priorities[i] > highestPriority) {
                if(currentLoad + binArr[i].FullLevel <= truckCapacity) {
                    highestPriority = priorities[i];
                    nextBin = i;
                }
            }
        }
        
        if(nextBin == -1) {
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
            printf("   TRUCK FULL! Cannot collect remaining priority bins.\n");
            printf("   Current Load: %d/%d units (%.1f%%)\n", 
                   currentLoad, truckCapacity, (currentLoad * 100.0) / truckCapacity);
            printf("   Remaining bins: %d\n", totalBins - binsCollected);
            break;
        }
        
        // Collect the highest priority bin
        visited[nextBin] = 1;
        *totalDist += dist[nextBin];
        currentLoad += binArr[nextBin].FullLevel;
        route[(*routeLen)++] = nextBin;
        binsCollected++;
        
        printf("Step %d: %s → %s (Priority: %d)\n", 
               step++, loc[currentPos].name, loc[nextBin].name, highestPriority);
        printf("        Distance: %d km | Fill Level: %d%%\n", 
               dist[nextBin], binArr[nextBin].FullLevel);
        printf("        Truck Load: %d/%d units | Priority Collection\n\n",
               currentLoad, truckCapacity);
        
        currentPos = nextBin;
    }
    
    // Return to depot
    dijkstra(n, adj, currentPos, dist, parent);
    *totalDist += dist[start];
    route[(*routeLen)++] = start;
    
    printf("Final Step: %s → %s (Return to Depot)\n", 
           loc[currentPos].name, loc[start].name);
    printf("             Distance: %d km\n", dist[start]);
    
    printf("\n============================================================\n");
    printf("               PRIORITY COLLECTION SUMMARY\n");
    printf("============================================================\n");
    printf("Complete Route: ");
    for(int i = 0; i < *routeLen; i++) {
        printf("%s", loc[route[i]].name);
        if(i < *routeLen - 1) printf(" → ");
    }
    printf("\n\n");
    printf("Total Distance: %d km\n", *totalDist);
    printf("Bins Collected: %d/%d\n", binsCollected, totalBins);
    printf("Waste Collected: %d units\n", currentLoad);
    printf("Truck Utilization: %.1f%%\n", (currentLoad * 100.0) / truckCapacity);
    
    if(binsCollected < totalBins) {
        printf("\n⚠️  Priority bins remaining: %d\n", totalBins - binsCollected);
        printf("   Consider dispatching additional truck.\n");
    } else {
        printf("\n✓ All priority bins collected successfully!\n");
    }
    printf("============================================================\n\n");
}