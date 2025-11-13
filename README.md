# SmartCollector 🚛♻️

**SmartCollector** is an intelligent garbage collection management system built in C that optimizes waste collection routes using **Dijkstra's algorithm** and **greedy approach**. The system efficiently manages trucks, tracks bin fill levels, and plans optimal collection routes for cities.

---

## 🎯 Project Overview

This system simulates a real-world garbage collection scenario where:
- Multiple locations in a city have garbage bins with varying fill levels
- Trucks with different capacities are dispatched to collect waste
- Routes are optimized using **shortest path algorithms**
- Load tracking ensures trucks don't exceed capacity
- System alerts when additional trucks are needed

---

## ✨ Current Features (Task 1 ✅ Complete)

### **Task 1: Route Planning with Greedy Approach** ✅
- ✅ **Dijkstra's Algorithm**: Calculates shortest paths between all locations
- ✅ **Greedy Nearest-Neighbor**: Selects closest bin at each step
- ✅ **Load Tracking**: Monitors truck capacity in real-time
- ✅ **Smart Collection**: Only collects bins that fit in truck
- ✅ **Route Visualization**: Displays step-by-step collection route
- ✅ **Return to Depot**: Completes full circuit back to starting point

**File**: `dijkstra.c`, `dijkstra.h`

---

## 🚧 Upcoming Features (Tasks 2-4)

### **Task 2: Multi-Truck Dispatch System** ⏳
**Objective**: Automatically dispatch multiple trucks when single truck cannot handle all bins.

**What to Implement**:
- Check if total bin load exceeds largest truck capacity
- Divide bins among multiple trucks (by location clusters or load)
- Call `greedyCollectionRoute()` for each truck separately
- Display routes for all dispatched trucks

**Implementation**:
- Create new file: `multitruck.c`, `multitruck.h`
- Add function: `void dispatchMultipleTrucks(int num, int adj[][num], int startDepot, bin binArr[], location loc[], truck arrTruck[], int truckCount)`
- Call this from `graph.c` instead of single truck route

**Integration Point** (in `graph.c`):
```c
// Replace single truck call with:
dispatchMultipleTrucks(num, adj, startPosition, bin, location, arrTruck, truck_num);
```

---

### **Task 3: Auto-Refill Simulation** ⏳
**Objective**: Simulate real-time bin refilling and automatic truck dispatch.

**What to Implement**:
- Timer system where 1 second = 1 day
- Gradually increase bin fill levels over time (e.g., 5% per day)
- After truck collects bins, reset collected bins to 0%
- Auto-dispatch trucks when average bin fill reaches threshold (e.g., 80%)
- Display real-time bin status updates

**Implementation**:
- Create new file: `simulation.c`, `simulation.h`
- Add functions:
  - `void startSimulation(bin binArr[], int num, int duration)`
  - `void updateBinLevels(bin binArr[], int num, int daysPassed)`
  - `int checkDispatchThreshold(bin binArr[], int num)` // Returns 1 if dispatch needed
- Use `sleep(1)` for 1-second intervals
- Call truck dispatch when threshold reached

**Integration Point** (in `graph.c`):
```c
// After initial collection, start simulation:
startSimulation(bin, num, 30); // Run for 30 days
```

---

### **Task 4: Priority-Based Collection** ⏳
**Objective**: Prioritize bins with higher fill levels instead of just closest distance.

**What to Implement**:
- Calculate priority score for each bin: `priority = fillLevel / distance`
- Sort bins by urgency (high fill % = high priority)
- Modify route selection to collect urgent bins first
- Handle cases where high-priority bin is far but critical

**Implementation**:
- Create new file: `priority.c`, `priority.h`
- Add functions:
  - `float calculatePriority(int fillLevel, int distance)`
  - `int selectNextBin(bin binArr[], int dist[], int visited[], int num)` // Returns highest priority bin
  - `void sortBinsByUrgency(bin binArr[], int indices[], int count)`
- Modify the greedy selection logic in route planning

**Integration Point** (modify `dijkstra.c`):
```c
// In greedyCollectionRoute(), replace:
// "find closest bin" with:
nextBin = selectNextBin(binArr, dist, visited, n);
```

---

## 📁 Project Structure

```
SmartCollector/
├── graph.c              # Main program, user input, integration
├── dijkstra.c           # Task 1: Dijkstra + Greedy Route (✅ Complete)
├── dijkstra.h           # Header for dijkstra.c
├── truck.c              # Truck management, min-heap
├── truck.h              # Header for truck.c
├── multitruck.c         # Task 2: Multi-truck dispatch (TODO)
├── multitruck.h         # Header for Task 2
├── simulation.c         # Task 3: Auto-refill simulation (TODO)
├── simulation.h         # Header for Task 3
├── priority.c           # Task 4: Priority-based collection (TODO)
├── priority.h           # Header for Task 4
└── README.md            # Project documentation
```

---

## 🛠️ Tech Stack

- **Language**: C
- **Algorithms**: Dijkstra's Shortest Path, Greedy Nearest-Neighbor
- **Data Structures**: Graphs (Adjacency Matrix), Min-Heap, Arrays
- **Platform**: Linux (GCC compatible)

---

## 🚀 How to Run

### Compile:
```bash
gcc graph.c dijkstra.c truck.c -o proj
```

### Run:
```bash
./proj
```

### As Tasks 2-4 are completed, compile with:
```bash
gcc graph.c dijkstra.c truck.c multitruck.c simulation.c priority.c -o proj
```

---

## 📊 Example Usage

```
Enter the number of locations -> 5

Enter the name of location 0 -> Depot
Does this location have a Bin? (1 = Yes / 0 = No): 0

Enter the name of location 1 -> Market
Does this location have a Bin? (1 = Yes / 0 = No): 1
Enter the fill level (%) of bin at Market -> 85

[... enter distances between locations ...]

Enter the starting position -> 0

✓ Truck-3 assigned for collection
  Capacity: 100 units

============================================================
              GREEDY COLLECTION ROUTE PLANNING
============================================================
Starting from: Depot
Truck Capacity: 100 units
Total bins to collect: 3

Step 1: Depot ➜ Market
        Distance: 10 km  |  Bin Fill Level: 85 units
        Truck Load: 85 / 100 units (85.0% full)
        Status: ✓ Bin collected successfully

[... continues with route ...]
```

---

## 📋 Task Assignment Guide

### For Task 2 (Multi-Truck):
1. Create `multitruck.c` and `multitruck.h`
2. Implement bin division logic among trucks
3. Call `greedyCollectionRoute()` for each truck
4. **Don't modify** `dijkstra.c` or `truck.c`
5. Update `graph.c` only to call your new function

### For Task 3 (Simulation):
1. Create `simulation.c` and `simulation.h`
2. Use `sleep()` for time intervals
3. Call existing truck dispatch functions
4. **Don't modify** core routing logic
5. Update `graph.c` to start simulation after initial run

### For Task 4 (Priority):
1. Create `priority.c` and `priority.h`
2. Implement priority calculation functions
3. Provide `selectNextBin()` function for route planning
4. Can modify `dijkstra.c` only in bin selection part
5. Keep Dijkstra algorithm itself unchanged

---

## 🎓 Design & Analysis of Algorithms (DAA) Components

### Algorithms Used:
- **Dijkstra's Algorithm**: O(V²) - Finds shortest paths from depot
- **Greedy Approach**: O(V² × B) - Nearest-neighbor bin selection
- **Min-Heap**: O(log T) - Efficient truck selection

### Why These Algorithms?
- **Dijkstra over Floyd-Warshall**: Single starting point makes Dijkstra more efficient
- **Greedy over TSP**: NP-hard problem approximated with practical O(n²) solution
- **Min-Heap**: Ensures smallest suitable truck is always selected first

---

## 👥 Team

**Aman Rautela** - Task 1 (Route Planning) ✅  
**Sudhanshu Dhasmana** - Task 2 (Multi-Truck Dispatch) ✅    
**Anjali Joshi** - Task 3 (Priority-Based Collection) ✅  
**Shivangi Sati** - Task 4 (Auto-Refill Simulation) ✅    
 

**Institution**: MCA @ GEHU

---

## 📝 License

This project is licensed under the MIT License.

---

## 🤝 Contributing

Each task should be implemented in separate files without modifying existing core logic. Follow the integration points mentioned above for each task.

---

**Happy Coding! 🚛💨**

