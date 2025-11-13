#include <stdio.h>
#include "dijkstra.h"   /* safe: includes dijkstra.h which uses guards */
#include "truck.h"

void heapify(truck arr[], int n, int i);

void minHeap(truck arr[], int n){
    for(int i = n/2 - 1; i >= 0; i--){
        heapify(arr, n, i);
    }
}

void heapify(truck arr[], int n, int i){
        int smallest = i;
        int left = 2*i + 1;
        int right = 2*i + 2;

        if(left < n && arr[left].capacity < arr[smallest].capacity){
            smallest = left;
        }
        if(right < n && arr[right].capacity < arr[smallest].capacity){
            smallest = right;
        }

        if(i != smallest){
            truck temp = arr[i];
            arr[i] = arr[smallest];
            arr[smallest] = temp;

            heapify(arr, n, smallest);
        }
}

void initTruck(truck arr[], int n){
    int capacity[] = {75, 25, 100, 50};
    for(int i = 0; i < n; i++){
        arr[i].capacity = capacity[i];
    }

    minHeap(arr, n);

    for(int i = 0; i < n; i++){
        arr[i].currentLoad = 0;
        arr[i].position = 0;
    }
}

void displayTrucks(int n, truck arr[]){
    printf("\n---------------TRUCK STATUS-------------\n");
    for(int i = 0 ; i < n; i++){
        printf(" Truck -> %d | Capacity -> %d | Load -> %d\n", i+1, arr[i].capacity, arr[i].currentLoad);
    }
}

int selectTruck(int num, int n, truck arr[], bin binArr[]){
    int totalLoad = 0;
    for(int i = 0; i < num; i++){
        if(binArr[i].hasBin){
            totalLoad += binArr[i].FullLevel;
        }
    }

    for(int i = 0; i < n; i++){
        if(arr[i].capacity >= totalLoad){
            return i;
        }
    }

    int largestTruckIndex = 0;
    int maxCapacity = arr[0].capacity;

    for(int i = 1; i < n; i++){
        if(arr[i].capacity > maxCapacity){
            maxCapacity = arr[i].capacity;
            largestTruckIndex = i;
        }
    }

    return largestTruckIndex;
}
