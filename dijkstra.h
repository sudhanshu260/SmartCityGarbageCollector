#ifndef DIJKSTRA_H
#define DIJKSTRA_H

/* Protect type definitions so multiple headers can include them
   without causing redefinition errors. */
#ifndef LOCATION_TYPE_DEFINED
#define LOCATION_TYPE_DEFINED
typedef struct location{
    int vertice;
    char name[50];
} location;
#endif

#ifndef BIN_TYPE_DEFINED
#define BIN_TYPE_DEFINED
typedef struct Bin{
    int hasBin;
    int FullLevel;
} bin;
#endif

/* Dijkstra's algorithm prototype */
void dijkstra(int n, int adj[][n], int start, int dist[], int parent[]);

/* Greedy collection route prototype */
void greedyCollectionRoute(int n, int adj[][n], int start, bin binArr[],
                          location loc[], int route[], int *routeLen,
                          int *totalDist, int truckCapacity);

#endif /* DIJKSTRA_H */
