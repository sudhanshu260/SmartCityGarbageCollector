#include <stdio.h>
#include "truck.h"
#include "dijkstra.h"
#include "multitruck.h"
#include "priority.h"

void displayMenu() {
    printf("\n\n=========================================\n");
    printf("        SMART COLLECTOR SYSTEM\n");
    printf("=========================================\n");
    printf("1. Single Truck Greedy Collection\n");
    printf("2. Multi-Truck Dispatch\n");
    printf("3. Priority-Based Collection\n");
    printf("4. Exit\n");
    printf("Choose an option (1-4): ");
}

int main() {
    int num = 0;
    printf("\n\n\t\t=========================================\n");
    printf("\t\t        ROADMAP OF DEHRADUN\n");
    printf("\t\t=========================================\n\n");

    printf("Enter the number of locations -> ");
    scanf("%d", &num);

    location location[num];
    int adj[num][num];
    bin bin[num];

    // Input locations and bins
    for (int i = 0; i < num; i++) {
        location[i].vertice = i;
        printf("\nEnter the name of location %d -> ", i);
        scanf("%s", location[i].name);

        printf("Does this location have a Bin? (1 = Yes / 0 = No): ");
        scanf("%d", &bin[i].hasBin);

        if (bin[i].hasBin) {
            printf("Enter the fill level (%%) of bin at %s -> ", location[i].name);
            scanf("%d", &bin[i].FullLevel);
        } else {
            bin[i].FullLevel = 0; 
        }
    }

    // Input distance matrix
    printf("\n\n============================================\n");
    printf("  Enter the distance between locations (0 if none)\n");
    printf("============================================\n\n");

    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num; j++) {
            printf("Distance between %-12s and %-12s -> ", location[i].name, location[j].name);
            scanf("%d", &adj[i][j]);
        }
        printf("\n");
    }

    // Display city information
    printf("\n\n============================================================\n");
    printf("                   CITY AND BIN INFORMATION\n");
    printf("============================================================\n\n");

    printf("%-5s %-15s %-10s %-10s\n", "No.", "Location", "Bin", "Fill Level");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < num; i++) {
        printf("%-5d %-15s %-10s %d%%\n",location[i].vertice,location[i].name,
               (bin[i].hasBin ? "YES" : "NO"),bin[i].FullLevel);
    }

    // Display distance matrix
    printf("\n\n\t\t=========================================\n");
    printf("\t\t            DISTANCE MATRIX\n");
    printf("\t\t=========================================\n\n");

    printf("%-15s", " ");
    for (int i = 0; i < num; i++) {
        printf("%-15s", location[i].name);
    }
    printf("\n");

    for (int i = 0; i < num; i++) {
        printf("%-15s", location[i].name);
        for (int j = 0; j < num; j++) {
            printf("%-15d", adj[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    
    // Initialize trucks
    truck arrTruck[truck_num];
    initTruck(arrTruck, truck_num);
    displayTrucks(truck_num, arrTruck);

    int choice;
    int startPosition;
    
    do {
        displayMenu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: {
                printf("\nEnter the starting position (0-%d): ", num-1);
                scanf("%d", &startPosition);
                
                int truckNum = selectTruck(num, truck_num, arrTruck, bin);
                
                if(truckNum != -1) {
                    int route[num];
                    int routeLen = 0;
                    int totalDist = 0;
                    
                    greedyCollectionRoute(num, adj, startPosition, bin, location, 
                                         route, &routeLen, &totalDist, 
                                         arrTruck[truckNum].capacity);
                }
                break;
            }
            
            case 2: {
                printf("\nEnter the starting position (0-%d): ", num-1);
                scanf("%d", &startPosition);
                
                MultiTruckPlan plans[truck_num];
                int numPlans = 0;
                
                multiTruckDispatch(num, adj, startPosition, bin, location, 
                                 arrTruck, truck_num, plans, &numPlans);
                
                displayMultiTruckPlan(plans, numPlans, location);
                break;
            }
            
            case 3: {
                printf("\nEnter the starting position (0-%d): ", num-1);
                scanf("%d", &startPosition);
                
                int truckNum = selectTruck(num, truck_num, arrTruck, bin);
                
                if(truckNum != -1) {
                    int route[num];
                    int routeLen = 0;
                    int totalDist = 0;
                    
                    priorityBasedCollection(num, adj, startPosition, bin, location, 
                                           route, &routeLen, &totalDist, 
                                           arrTruck[truckNum].capacity);
                }
                break;
            }
            
            case 4:
                printf("\nThank you for using Smart Collector System!\n");
                break;
                
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
        
    } while(choice != 4);

    return 0;
}