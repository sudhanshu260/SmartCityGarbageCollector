#include <stdio.h>
#include <limits.h>
#include "truck.h"    /* safe: truck.h uses guards for types */
#include "dijkstra.h"

void dijkstra(int n, int adj[][n], int start, int dist[], int parent[]){
    int visited[n];
    for(int i = 0 ; i < n ;i++){
        visited[i] = 0;
        parent[i] = -1;
        dist[i] = INT_MAX;
    }

    dist[start] = 0;
    for(int i = 0 ; i < n ; i++){
        int unVisited = -1;
        for(int j = 0 ; j < n ; j++){
            if(!visited[j] && (unVisited == -1 || dist[j] < dist[unVisited])){
                unVisited = j;
            }
        }
        if(unVisited == -1) break;

        visited[unVisited] = 1;

        for(int k = 0 ; k < n ; k++){
            /* treat 0 as "no edge" like your original code */
            if(adj[unVisited][k] != 0 && !visited[k] && dist[unVisited] != INT_MAX &&
               dist[unVisited] + adj[unVisited][k] < dist[k]){
                dist[k] = dist[unVisited] + adj[unVisited][k];
                parent[k] = unVisited;
            }
        }
    }
}

/* greedyCollectionRoute: uses dijkstra() to find nearest unvisited bin that fits truck */
void greedyCollectionRoute(int n, int adj[][n], int start, bin binArr[],
                          location loc[], int route[], int *routeLen,
                          int *totalDist, int truckCapacity) {

    int visited[n];
    for(int i = 0; i < n; i++) {
        visited[i] = 0;
    }

    int currentPos = start;
    int currentLoad = 0;
    *routeLen = 0;
    *totalDist = 0;

    route[(*routeLen)++] = currentPos;
    visited[currentPos] = 1;

    int totalBins = 0;
    int totalBinLoad = 0;
    for(int i = 0; i < n; i++) {
        if(binArr[i].hasBin && i != start) {
            totalBins++;
            totalBinLoad += binArr[i].FullLevel;
        }
    }

    printf("\n\n============================================================\n");
    printf("              GREEDY COLLECTION ROUTE PLANNING\n");
    printf("============================================================\n");
    printf("Starting from: %s\n", loc[start].name);
    printf("Truck Capacity: %d units\n", truckCapacity);
    printf("Total bins in city: %d\n", totalBins);
    printf("Total waste to collect: %d units\n\n", totalBinLoad);

    if(totalBinLoad > truckCapacity) {
        printf("⚠️  WARNING: Total load (%d) exceeds truck capacity (%d)\n",
               totalBinLoad, truckCapacity);
        printf("    Truck will collect as much as possible.\n");
        printf("    Remaining bins will need another truck (Task 2(person) will handlel this).\n\n");
    }

    int stepCount = 1;
    int binsCollected = 0;
    int binsRemaining = totalBins;

    while(binsRemaining > 0) {
        int dist[n], parent[n];
        dijkstra(n, adj, currentPos, dist, parent);

        int nextBin = -1;
        int minDist = INT_MAX;

        for(int i = 0; i < n; i++) {
            if(binArr[i].hasBin && !visited[i] && dist[i] < minDist) {
                if(currentLoad + binArr[i].FullLevel <= truckCapacity) {
                    minDist = dist[i];
                    nextBin = i;
                }
            }
        }

        if(nextBin == -1) {
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
            printf("   TRUCK FULL! Cannot collect remaining bins.\n");
            printf("   Current Load: %d / %d units (%.1f%% full)\n",
                   currentLoad, truckCapacity, (currentLoad * 100.0) / truckCapacity);
            printf("   Bins still uncollected: %d\n", binsRemaining);
            printf("   ➡️  Another truck dispatch required (Task 2 (person) will handles this)\n");
            printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
            break;
        }

        visited[nextBin] = 1;
        *totalDist += dist[nextBin];
        currentLoad += binArr[nextBin].FullLevel;
        route[(*routeLen)++] = nextBin;
        binsCollected++;

        printf("Step %d: %s -> %s\n", stepCount++, loc[currentPos].name, loc[nextBin].name);
        printf("        Distance: %d km  |  Bin Fill Level: %d units\n",
               dist[nextBin], binArr[nextBin].FullLevel);
        printf("        Truck Load: %d / %d units (%.1f%% full)\n",
               currentLoad, truckCapacity, (currentLoad * 100.0) / truckCapacity);
        printf("        Status:  Bin collected successfully\n\n");

        currentPos = nextBin;
        binsRemaining--;
    }

    int dist[n], parent[n];
    dijkstra(n, adj, currentPos, dist, parent);
    /* add return-to-depot distance (if edge exists; if 0 treated as none,
       this will add 0 like your original convention) */
    *totalDist += dist[start];
    route[(*routeLen)++] = start;

    printf("Step %d: %s -> %s (Return to Depot)\n", stepCount, loc[currentPos].name, loc[start].name);
    printf("        Distance: %d km\n", dist[start]);
    printf("        Status:  Returned to depot\n");

    printf("\n============================================================\n");
    printf("                    ROUTE SUMMARY\n");
    printf("============================================================\n");
    printf("Complete Route: ");
    for(int i = 0; i < *routeLen; i++) {
        printf("%s", loc[route[i]].name);
        if(i < *routeLen - 1) printf(" -> ");
    }
    printf("\n\n");
    printf("    Stats:\n");
    printf("    Total Distance Traveled: %d km\n", *totalDist);
    printf("    Bins Collected: %d / %d\n", binsCollected, totalBins);
    printf("    Waste Collected: %d / %d units\n", currentLoad, totalBinLoad);
    if (truckCapacity > 0)
        printf("    Truck Utilization: %.1f%%\n", (currentLoad * 100.0) / truckCapacity);
    else
        printf("    Truck Utilization: N/A (capacity 0)\n");

    if(binsRemaining > 0) {
        printf("\n⚠️  INCOMPLETE COLLECTION:\n");
        printf("    Bins not collected: %d\n", binsRemaining);
        printf("    Remaining waste: %d units\n", totalBinLoad - currentLoad);
        printf("    Action needed: Dispatch additional truck (Task 2(person) will handel this)\n");
    } else {
        printf("\n ALL BINS COLLECTED SUCCESSFULLY!\n");
    }
    printf("============================================================\n\n");
}
