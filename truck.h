#ifndef TRUCK_H
#define TRUCK_H

#define truck_num 4

/* Protect types to avoid redefinition when included alongside dijkstra.h */
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

typedef struct Truck{
    int capacity;
    int currentLoad;
    int position;
} truck;

/* prototypes */
void initTruck(truck arr[], int n);
void displayTrucks(int n, truck arr[]);
int selectTruck(int num, int n, truck arr[], bin binArr[]);

#endif /* TRUCK_H */
