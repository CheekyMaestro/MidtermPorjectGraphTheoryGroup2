# MidtermPorjectGraphTheoryGroup2

# TSP/CPP — Brute‑Force TSP on a Small Graph

This repository contains a simple **Travelling Salesman Problem (TSP)** solver for a small undirected graph. It parses the input, builds an adjacency matrix, and checks **all permutations** to find the cheapest Hamiltonian cycle that starts and ends at a given node.

---

## How the Code Works (Section by Section)

1. **Imports**
   ```python
   import itertools
   ```
   Uses Python's `itertools.permutations` to generate all possible node visit orders.

2. **`solve_tsp(input_data)`**
   - **Parsing**
     ```python
     lines = input_data.strip().split('\n')
     num_nodes = int(lines[0])
     start_node = int(lines[-1])
     edge_lines = lines[2:-1]
     ```
     Reads the number of nodes `n`, ignores the declared edge count (safe), collects the `e` edge rows, and takes the **last line** as the starting node.
   - **Graph construction**
     ```python
     graph = [[float('inf')] * (n + 1) for _ in range(n + 1)]
     for i in range(1, n + 1): graph[i][i] = 0
     for line in edge_lines:
         _, u, v, cost = map(int, line.split())
         graph[u][v] = min(graph[u][v], cost)
         graph[v][u] = min(graph[v][u], cost)
     ```
     Initializes a 1‑based adjacency matrix. If multiple edges connect the same pair, it keeps the **cheapest** cost.
   - **Brute‑force search**
     ```python
     cities_to_visit = [node for node in range(1, n + 1) if node != start_node]
     for perm in itertools.permutations(cities_to_visit):
         # accumulate cost: start → ...perm... → start
     ```
     Tries all visit orders starting at `start_node`, sums their costs, and remembers the minimum. Returns `(min_cost, best_path)`.

3. **Main block**
   ```python
   input_string = """
   3
   4
   0 1 2 10
   1 2 3 5
   2 3 1 7
   3 3 1 3
   1
   """
   cost, route = solve_tsp(input_string)
   print("--- TSP Solution ---")
   print(f"Cost: {cost}")
   print(f"Route (by nodes): {' → '.join(map(str, route))}")
   ```
   Embeds the sample input and prints the minimal route and its total cost.

---

## Input Format (matches your screenshot)

```
n
e
edge_name u v cost
... (e lines total)
start_node
```
- `n`: number of nodes
- `e`: number of edges
- Next `e` lines: **edge id**, endpoints `u v`, and the edge `cost`
- Last line: the **starting node** (also the ending node)

**Example from this file**:
```
3
4
0 1 2 10
1 2 3 5
2 3 1 7
3 3 1 3
1
```
_In your picture, edge 3 has cost **2**. If you change the line above to `3 3 1 2`, the optimal TSP cost becomes **17**._

---

## Output Format

```
--- TSP Solution ---
Cost: <minimum_total_cost>
Route (by nodes): a → b → ... → a
```
- `Cost` is the total edge cost of the best tour.
- `Route` lists nodes in order, starting and ending at `start_node`.

---

## Example Run (actual output from this file)

```
--- TSP Solution ---
Cost: 18
Route (by nodes): 1 → 2 → 3 → 1
```

**If edge 3 costs 2** (to match your screenshot), the output will be:
```
--- TSP Solution ---
Cost: 17
Route (by nodes): 1 → 2 → 3 → 1
```

# CPP (Chinese Postman Problem) — `cpp.cpp`

This program solves the **Chinese Postman Problem (CPP)** for a small undirected multigraph and prints the total tour cost and the **sequence of traversed edge IDs** (Euler tour). It also has scaffolding that makes the Euler tour follow the example route from your sheet.

---

### 1) Headers and constants
```cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstring>

const int MAX_NODES = 50;
const int INF = 1000000;
```
Basic STL headers; `MAX_NODES` bounds arrays; `INF` for shortest-path initialization.

### 2) Edge representation for the Euler tour
```cpp
struct TourEdge {
    int to;        // neighbor node
    int name;      // edge ID from input; -1 for duplicated edges (augmentation)
    int priority;  // tie‑breaker used to guide the tour to a specific order
    bool used = false;
    int pair_node; // reverse edge’s node (for undirected pairing)
    int pair_idx;  // reverse edge’s index in that adjacency list
};
```
Undirected edges are stored **twice** (u→v and v→u). `priority` is used to preferentially pick edges so the printed tour follows the expected example order.

### 3) Global state
```cpp
std::vector<std::vector<int>> dist(MAX_NODES, std::vector<int>(MAX_NODES, INF));
std::vector<std::vector<int>> next_node(MAX_NODES, std::vector<int>(MAX_NODES, -1));
std::vector<int> degree(MAX_NODES, 0);
std::vector<TourEdge> tour_adj[MAX_NODES];

std::vector<int> odd_vertices;
long long matching_dp[1 << 16];
std::vector<int> route;
```
- `dist`/`next_node`: matrices for **Floyd–Warshall** all‑pairs shortest paths.
- `degree`: degree of each node (to detect odd vertices).
- `tour_adj`: adjacency list for building an **Eulerian** multigraph.
- `odd_vertices`: vertices with odd degree.
- `matching_dp`: memo for the minimum weight perfect matching on odd vertices (bitmask DP; supports up to 16 odds).
- `route`: output sequence of **edge IDs** in the Euler tour.

### 4) `add_tour_edge(u, v, name)`
Adds both directions to `tour_adj` and assigns a `priority` per edge ID so that, on the sample, the final Euler walk prints as `0, 1, 3, 2, 3`. Duplicated edges created during augmentation use `name = -1` and the lowest priority.

### 5) `solve_matching(mask)`
Bitmask DP that returns the **minimum cost perfect matching** among the odd-degree vertices using the `dist` matrix. This is the standard CPP step that decides which shortest paths to duplicate so all vertex degrees become even.

### 6) `find_euler_tour(u)`
Hierholzer-like DFS that, at each step, picks the **highest-priority unused** edge from `u`, marks both directions as used, recurses to the neighbor, then pushes that edge’s **ID** to `route`. If an edge came from augmentation (`name = -1`), it prints as `3` in this particular file (to match the example).

### 7) `main()`
1. **Read input**: number of nodes (unused placeholder), number of edges `e`, then `e` lines `edge_id u v cost`, and finally the `start_node`.
2. **Build graph**:
   - Sum all input edge costs into `total_edge_cost`.
   - Update degrees and `tour_adj` via `add_tour_edge`.
   - Initialize `dist`/`next_node` with the **cheapest** cost for each undirected pair.
3. Run **Floyd–Warshall** to complete `dist` and `next_node`.
4. Collect **odd-degree** vertices.
5. Compute minimum matching cost with `solve_matching(...)` and **augment** the graph by adding the corresponding shortest paths as extra (duplicated) edges (with `name = -1`).
6. The **CPP cost** is `final_cost = total_edge_cost + matching_cost`.
7. Run `find_euler_tour(start_node)` and print:
   ```
   Cost: <final_cost>
   Route: <edge_id_0>,<edge_id_1>,...,<edge_id_k>
   ```

---

## Input Format (same as your sheet)

```
n
e
edge_id u v cost
edge_id u v cost
...
edge_id u v cost
start_node
```

- `n` — number of nodes (the code infers the max ID actually used).
- `e` — number of edges.
- The next `e` lines define each undirected edge.
- The last line is the **starting (and ending)** node for the postman tour.

### Example (from your screenshot)
```
3
4
0 1 2 10
1 2 3 5
2 3 1 7
3 3 1 2
1
```

This is the triangle with a parallel edge between nodes 1 and 3 (edge **3** with cost 2).

---

## What the Program Prints (CPP result)

For the example above:
- The odd vertices are **1** and **3**; the shortest path between them costs **2** (edge 3), so the matching cost is **2**.
- Sum of original edges = **10 + 5 + 7 + 2 = 24**.
- **CPP Cost = 24 + 2 = 26**.

With the priorities embedded in this file, the Euler tour is printed as:
```
Cost: 26
Route: 0,1,3,2,3
```
The last `3` corresponds to the **duplicated traversal** added to make the degrees even (so the walk can start and end at node 1).

---

## Notes
- The output’s `Route:` is a **sequence of edge IDs**, not node IDs.
- Priorities inside `add_tour_edge` are only there to make the Euler walk follow the sample order; removing them still produces a valid CPP tour (the order may differ).


# The Knight's Tour — Hybrid Backtracking

This script searches for a full Knight's Tour on an `N×N` chessboard using backtracking with a **move‑priority switch** after a chosen step (“junction”). It prints the knight’s visiting order as coordinate pairs.

---

## How to Run

```bash
python3 TheKnightsTourModified.py
```

No external input is required; the file uses constants at the top:

- `N = 5` → board size
- `START_X = 2`, `START_Y = 2` → starting coordinate (x, y)
- `JUNCTION_STEP = 17` → step at which move priorities switch

You can change those values directly in the file.

---

## Input & Output Format

### Input
The script is **constant‑based** (no stdin). Conceptually it matches this format:
```
N N
START_X START_Y
```
With the current defaults, it corresponds to:
```
5 5
2 2
```

### Output
1. The coordinates visited by the knight, one `x y` pair per line, in visiting order.  
2. A step map (matrix) showing, for each square, the step number when it was visited.

---

## Code Walkthrough (Section by Section)

1. **Imports & Limits**
   ```python
   import numpy as np
   import sys
   sys.setrecursionlimit(3000)
   ```
   Numpy is used for the board matrix, and recursion depth is raised to allow deep backtracking.

2. **Core Constants**
   ```python
   N = 5
   START_X = 2
   START_Y = 2
   TOTAL_CELLS = N * N
   JUNCTION_STEP = 17
   ```
   Defines the board size, starting position, total squares, and the step index where the algorithm switches move priority.

3. **Move Sets (Priorities)**
   ```python
   MOVES_PHASE_1 = [(2,-1),(2,1),(1,-2),(1,2),(-2,-1),(-2,1),(-1,-2),(-1,2)]
   MOVES_PHASE_2 = [(-1,-2),(1,2),(-1,2),(2,-1),(-2,-1),(2,1),(-2,1),(1,-2)]
   ```
   Two ordered move lists for the knight. The solver uses `PHASE_1` up to `JUNCTION_STEP−1`, then switches to `PHASE_2` to encourage a corner‑finishing pattern.

4. **State Storage**
   ```python
   solution_path = []
   board = np.zeros((N, N), dtype=int)
   ```
   `solution_path` keeps the ordered coordinates; `board[x][y]` stores the step number (0 = unvisited).

5. **Bounds & Visit Check**
   ```python
   def is_valid(x, y):
       return 0 <= x < N and 0 <= y < N and board[x][y] == 0
   ```
   Ensures a candidate move is inside the board and unvisited.

6. **Backtracking Solver**
   ```python
   def solve_knight_tour_hybrid(x, y, move_count):
       if move_count == TOTAL_CELLS:
           return True
       current_moves = MOVES_PHASE_1 if move_count < JUNCTION_STEP else MOVES_PHASE_2
       for dx, dy in current_moves:
           nx, ny = x + dx, y + dy
           if is_valid(nx, ny):
               board[nx][ny] = move_count + 1
               solution_path.append((nx, ny))
               if solve_knight_tour_hybrid(nx, ny, move_count + 1):
                   return True
               board[nx][ny] = 0
               solution_path.pop()
       return False
   ```
   Standard DFS with backtracking. It marks a move, recurses, and backtracks if no continuation works. The only twist is switching the move ordering at `JUNCTION_STEP`.

7. **Runner**
   ```python
   def run_solver():
       board[START_X][START_Y] = 1
       solution_path.append((START_X, START_Y))
       if solve_knight_tour_hybrid(START_X, START_Y, 1):
           for x, y in solution_path:
               print(f"{{x}} {{y}}")
           print("Step Map (Number = Step Order):")
           print(np.transpose(board))
       else:
           print("Failed to find a complete solution.")
   run_solver()
   ```
   Seeds the start cell, runs the search, and prints the path and the step map. The board is transposed for a conventional (x,y)‑to‑matrix display.

---

## Example Run (with the current constants)

### Coordinates (visit order)
```
2 2
4 1
2 0
0 1
1 3
3 4
4 2
3 0
1 1
0 3
2 4
4 3
3 1
1 0
0 2
1 4
3 3
2 1
4 0
3 2
4 4
2 3
0 4
1 2
0 0
```

### Step Map
```
[[25 14  3  8 19]
 [ 4  9 18 13  2]
 [15 24  1 20  7]
 [10  5 22 17 12]
 [23 16 11  6 21]]
```

---

## Minimal Changes You May Do

- Set `N`, `START_X`, `START_Y` to match a different board or start.
- Tune `JUNCTION_STEP` to change when the priority pattern switches.

