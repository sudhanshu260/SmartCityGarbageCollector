#ifndef SIMULATION_H
#define SIMULATION_H

#include "dijkstra.h"
#include "truck.h"  // Add this line
#include "multitruck.h"  // Add this line

#define REFILL_THRESHOLD 20  // Refill when bin reaches 20% capacity
#define SIMULATION_DAYS 7    // Simulate for 7 days

typedef struct SimulationDay {
    int day;
    int binsCollected;
    int wasteCollected;
    int distanceTraveled;
    int trucksUsed;
    int refillEvents;
} SimulationDay;

typedef struct RefillEvent {
    int day;
    int location;
    int oldLevel;
    int newLevel;
    char reason[50];
} RefillEvent;

// Auto-refill simulation functions
void simulateAutoRefill(int n, int adj[][n], int start, bin binArr[], 
                       location loc[], truck trucks[], int numTrucks);

void refillBins(int n, bin binArr[], location loc[], int day, 
                RefillEvent refillLog[], int *refillCount);

void simulateWasteGeneration(int n, bin binArr[], location loc[], int day);

void displaySimulationResults(SimulationDay results[], int days, 
                             RefillEvent refillLog[], int totalRefills);

// Helper functions
void initializeSimulationBins(int n, bin binArr[]);
int calculateDailyWaste(int location, int day);

#endif