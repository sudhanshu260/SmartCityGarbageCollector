#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simulation.h"

void initializeSimulationBins(int n, bin binArr[]) {
    srand(time(NULL));
    for(int i = 0; i < n; i++) {
        if(binArr[i].hasBin) {
            binArr[i].FullLevel = 30 + rand() % 60;
        }
    }
}

int calculateDailyWaste(int location, int day) {
    int baseWaste = 5 + rand() % 15;
    
    if(location % 2 == 1 && (day % 7 == 0 || day % 7 == 6)) {
        baseWaste += 10;
    }
    
    if(location % 2 == 0) {
        baseWaste = 8 + rand() % 10;
    }
    
    return baseWaste;
}

void simulateWasteGeneration(int n, bin binArr[], location loc[], int day) {
    printf("\n📊 Day %d - Waste Generation:\n", day);
    printf("────────────────────────────────────────────────────────────\n");
    
    for(int i = 0; i < n; i++) {
        if(binArr[i].hasBin) {
            int dailyWaste = calculateDailyWaste(i, day);
            int oldLevel = binArr[i].FullLevel;
            binArr[i].FullLevel += dailyWaste;
            
            if(binArr[i].FullLevel > 100) {
                binArr[i].FullLevel = 100;
            }
            
            printf("%-15s: %2d%% → %2d%% (+%d%%)\n", 
                   loc[i].name, oldLevel, binArr[i].FullLevel, dailyWaste);
        }
    }
}

void refillBins(int n, bin binArr[], location loc[], int day, 
                RefillEvent refillLog[], int *refillCount) {
    
    printf("\n🔄 Day %d - Auto Refill Check:\n", day);
    printf("────────────────────────────────────────────────────────────\n");
    
    int refillsToday = 0;
    
    for(int i = 0; i < n; i++) {
        if(binArr[i].hasBin && binArr[i].FullLevel >= 80) {
            RefillEvent *event = &refillLog[(*refillCount)++];
            event->day = day;
            event->location = i;
            event->oldLevel = binArr[i].FullLevel;
            binArr[i].FullLevel = 20;
            event->newLevel = binArr[i].FullLevel;
            snprintf(event->reason, 50, "High fill level (%d%%)", event->oldLevel);
            
            printf("🚨 HIGH PRIORITY: %s refilled %d%% → %d%%\n", 
                   loc[i].name, event->oldLevel, event->newLevel);
            refillsToday++;
            
        } else if(binArr[i].hasBin && binArr[i].FullLevel <= REFILL_THRESHOLD) {
            RefillEvent *event = &refillLog[(*refillCount)++];
            event->day = day;
            event->location = i;
            event->oldLevel = binArr[i].FullLevel;
            binArr[i].FullLevel = 100;
            event->newLevel = binArr[i].FullLevel;
            snprintf(event->reason, 50, "Empty bin refill");
            
            printf("✅ REFILL: %s refilled %d%% → %d%%\n", 
                   loc[i].name, event->oldLevel, event->newLevel);
            refillsToday++;
        }
    }
    
    if(refillsToday == 0) {
        printf("No refills needed - all bins at optimal levels\n");
    }
}

// Simple collection simulation without multi-truck dependency
void simulateDailyCollection(int n, bin binArr[], location loc[], int day,
                           SimulationDay *dailyResult) {
    
    printf("\n🚛 Day %d - Collection Simulation:\n", day);
    printf("────────────────────────────────────────────────────────────\n");
    
    int binsCollected = 0;
    int wasteCollected = 0;
    int trucksUsed = 0;
    
    // Simple collection logic
    int currentCapacity = 100; // truck capacity
    int currentLoad = 0;
    
    for(int i = 0; i < n; i++) {
        if(binArr[i].hasBin && binArr[i].FullLevel > 0) {
            if(currentLoad + binArr[i].FullLevel <= currentCapacity) {
                wasteCollected += binArr[i].FullLevel;
                currentLoad += binArr[i].FullLevel;
                binArr[i].FullLevel = 0; // Empty the bin
                binsCollected++;
                printf("Collected from %s: %d units\n", loc[i].name, binArr[i].FullLevel);
            } else {
                // Need another truck
                trucksUsed++;
                currentLoad = 0;
                printf("🚛 Truck %d full, dispatching new truck\n", trucksUsed);
            }
        }
    }
    
    if(binsCollected > 0) {
        trucksUsed++; // Count the last truck
    }
    
    dailyResult->binsCollected = binsCollected;
    dailyResult->wasteCollected = wasteCollected;
    dailyResult->trucksUsed = trucksUsed;
    dailyResult->distanceTraveled = binsCollected * 2 + 10; // Simple distance calculation
    dailyResult->refillEvents = 0; // Will be set later
}

void simulateAutoRefill(int n, int adj[][n], int start, bin binArr[], 
                       location loc[], truck trucks[], int numTrucks) {
    
    printf("\n\n============================================================\n");
    printf("              AUTO-REFILL SIMULATION SYSTEM\n");
    printf("============================================================\n");
    printf("Simulation Period: %d days\n", SIMULATION_DAYS);
    printf("Refill Threshold: %d%%\n", REFILL_THRESHOLD);
    printf("Starting Depot: %s\n\n", loc[start].name);
    
    initializeSimulationBins(n, binArr);
    SimulationDay results[SIMULATION_DAYS];
    RefillEvent refillLog[100];
    int totalRefills = 0;
    
    printf("🎯 INITIAL BIN STATUS:\n");
    printf("────────────────────────────────────────────────────────────\n");
    for(int i = 0; i < n; i++) {
        if(binArr[i].hasBin) {
            printf("%-15s: %3d%% fill level\n", loc[i].name, binArr[i].FullLevel);
        }
    }
    
    for(int day = 1; day <= SIMULATION_DAYS; day++) {
        printf("\n\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("                    DAY %d SIMULATION\n", day);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        
        // Step 1: Generate daily waste
        simulateWasteGeneration(n, binArr, loc, day);
        
        // Step 2: Check and perform auto-refills
        refillBins(n, binArr, loc, day, refillLog, &totalRefills);
        
        // Step 3: Perform collection
        simulateDailyCollection(n, binArr, loc, day, &results[day-1]);
        results[day-1].day = day;
        
        // Count refill events for this day
        results[day-1].refillEvents = 0;
        for(int i = 0; i < totalRefills; i++) {
            if(refillLog[i].day == day) {
                results[day-1].refillEvents++;
            }
        }
        
        printf("\n📈 Day %d Summary:\n", day);
        printf("   • Bins Collected: %d\n", results[day-1].binsCollected);
        printf("   • Waste Collected: %d units\n", results[day-1].wasteCollected);
        printf("   • Distance Traveled: %d km\n", results[day-1].distanceTraveled);
        printf("   • Trucks Used: %d\n", results[day-1].trucksUsed);
        printf("   • Refill Events: %d\n", results[day-1].refillEvents);
    }
    
    displaySimulationResults(results, SIMULATION_DAYS, refillLog, totalRefills);
}

void displaySimulationResults(SimulationDay results[], int days, 
                             RefillEvent refillLog[], int totalRefills) {
    
    printf("\n\n============================================================\n");
    printf("                 SIMULATION RESULTS SUMMARY\n");
    printf("============================================================\n");
    
    printf("\n📅 DAILY STATISTICS:\n");
    printf("────────────────────────────────────────────────────────────\n");
    printf("Day | Bins | Waste | Distance | Trucks | Refills\n");
    printf("────┼──────┼───────┼──────────┼────────┼─────────\n");
    
    int totalBins = 0, totalWaste = 0, totalDistance = 0, totalTrucks = 0;
    
    for(int i = 0; i < days; i++) {
        printf("%3d | %4d | %5d | %8d | %6d | %7d\n",
               results[i].day, results[i].binsCollected, results[i].wasteCollected,
               results[i].distanceTraveled, results[i].trucksUsed, results[i].refillEvents);
        
        totalBins += results[i].binsCollected;
        totalWaste += results[i].wasteCollected;
        totalDistance += results[i].distanceTraveled;
        totalTrucks += results[i].trucksUsed;
    }
    
    printf("────┼──────┼───────┼──────────┼────────┼─────────\n");
    printf("Total| %4d | %5d | %8d | %6d |\n", totalBins, totalWaste, totalDistance, totalTrucks);
    
    printf("\n🔄 REFILL EVENTS SUMMARY (%d total):\n", totalRefills);
    printf("────────────────────────────────────────────────────────────\n");
    
    if(totalRefills > 0) {
        for(int i = 0; i < totalRefills && i < 10; i++) {
            printf("Day %d: Location %d - %d%% → %d%% (%s)\n",
                   refillLog[i].day, refillLog[i].location + 1,
                   refillLog[i].oldLevel, refillLog[i].newLevel, refillLog[i].reason);
        }
        if(totalRefills > 10) {
            printf("... and %d more refill events\n", totalRefills - 10);
        }
    } else {
        printf("No refill events during simulation\n");
    }
    
    printf("\n📊 EFFICIENCY METRICS:\n");
    printf("────────────────────────────────────────────────────────────\n");
    printf("Average Daily Collection: %.1f bins/day\n", (float)totalBins / days);
    printf("Average Waste Collected: %.1f units/day\n", (float)totalWaste / days);
    printf("Average Distance: %.1f km/day\n", (float)totalDistance / days);
    printf("Truck Utilization: %.1f trucks/day\n", (float)totalTrucks / days);
    printf("Refill Rate: %.1f events/day\n", (float)totalRefills / days);
    
    printf("\n============================================================\n");
    printf("               SIMULATION COMPLETE!\n");
    printf("============================================================\n\n");
}