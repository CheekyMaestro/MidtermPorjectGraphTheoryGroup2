import numpy as np
import sys


sys.setrecursionlimit(3000)


N = 5             
START_X = 2       
START_Y = 2       
TOTAL_CELLS = N * N
JUNCTION_STEP = 17



#Knights tour movre

MOVES_PHASE_1 = [
    (2, -1),  # Right 2, Up 1
    (2, 1),   # Right 2, Down 1
    (1, -2),  # Right 1, Up 2
    (1, 2),   # Right 1, Down 2
    (-2, -1), # Left 2, Up 1
    (-2, 1),  # Left 2, Down 1
    (-1, -2), # Left 1, Up 2
    (-1, 2)   # Left 1, Down 2
]

#Fixed Directional Heuristic 

MOVES_PHASE_2 = [
    (-1, -2), 
    (1, 2), (-1, 2), (2, -1), (-2, -1), 
    (2, 1), (-2, 1), (1, -2)
]

# Global Variables
solution_path = []
board = np.zeros((N, N), dtype=int)


def is_valid(x, y):
    
    #Check if coordinates (x, y) are within bounds and not yet visited.
    
    return 0 <= x < N and 0 <= y < N and board[x][y] == 0


def solve_knight_tour_hybrid(x, y, move_count):
    
    
    #Backtracking function with logic to switch MOVES set at step 17.
    
    global solution_path, board

    if move_count == TOTAL_CELLS:
        return True

    # Logical change of MOVES
    if move_count < JUNCTION_STEP:
        # Use general priority (Phase 1)
        current_moves = MOVES_PHASE_1
    else:
        # Use specific priority for corner pattern (Phase 2)
        current_moves = MOVES_PHASE_2

    # RECURSION 
    for dx, dy in current_moves:
        next_x, next_y = x + dx, y + dy

        if is_valid(next_x, next_y):
            # Mark the square
            board[next_x][next_y] = move_count + 1
            solution_path.append((next_x, next_y))

            # Call recursively
            if solve_knight_tour_hybrid(next_x, next_y, move_count + 1):
                return True

            # BACKTRACKING
            board[next_x][next_y] = 0
            solution_path.pop()

    return False


def run_solver():

    """
    Set up and run the solution search process.
    """
    global solution_path, board

    # 1. Initialize
    board[START_X][START_Y] = 1
    solution_path.append((START_X, START_Y))

    print(f"Searching for Knight's Tour solution ({N}x{N} starting from {START_X}, {START_Y})...")
    print(f"Using Hybrid Backtracking (priority switch at step {JUNCTION_STEP}).")

    # 2. Begin solving
    if solve_knight_tour_hybrid(START_X, START_Y, 1):
        # 3. Show results
        for x, y in solution_path:
            print(f"{x} {y}")
        print("Step Map (Number = Step Order):")
        print(np.transpose(board))
    else:
        print("Failed to find a complete solution.")

# Run the solver
run_solver()
