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

