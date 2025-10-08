import itertools

def solve_tsp(input_data):
    lines = input_data.strip().split('\n')

    # 1. Parse the input
    num_nodes = int(lines[0])
    # num_edges is not strictly needed for this approach but good practice to read
    # num_edges = int(lines[1]) 
    start_node = int(lines[-1])

    # 2. Build the graph representation (Adjacency Matrix)
    # Initialize a matrix with infinity to represent no direct path.
    # We use (num_nodes + 1) to allow for 1-based indexing (nodes 1, 2, 3).
    graph = [[float('inf')] * (num_nodes + 1) for _ in range(num_nodes + 1)]

    # Set the cost from a node to itself as 0
    for i in range(1, num_nodes + 1):
        graph[i][i] = 0

    # Populate the matrix with costs from the edge list
    edge_lines = lines[2:-1]
    for line in edge_lines:
        _, u, v, cost = map(int, line.split())
        # If there's a cheaper road, use it. The graph is undirected.
        graph[u][v] = min(graph[u][v], cost)
        graph[v][u] = min(graph[v][u], cost)

    # 3. Find the optimal tour by checking all permutations
    nodes = list(range(1, num_nodes + 1))
    cities_to_visit = [node for node in nodes if node != start_node]

    min_cost = float('inf')
    best_path = None

    # Generate all possible orderings of the cities to visit
    for permutation in itertools.permutations(cities_to_visit):
        current_cost = 0
        
        # Start from the designated starting node
        last_node = start_node
        
        # Calculate cost for the current permutation
        for city in permutation:
            current_cost += graph[last_node][city]
            last_node = city
        
        # Add the cost to return to the start node
        current_cost += graph[last_node][start_node]

        # If we found a new shorter path, update the minimum cost and best path
        if current_cost < min_cost:
            min_cost = current_cost
            best_path = [start_node] + list(permutation) + [start_node]
            
    return min_cost, best_path


# --- Main execution ---
# Paste the input from the problem here
input_string = """
3
4
0 1 2 10
1 2 3 5
2 3 1 7
3 3 1 3
1
"""

# Solve the problem
cost, route = solve_tsp(input_string)

# Print the results
print("--- TSP Solution ---")
if route:
    print(f"Cost: {cost}")
    print(f"Route (by nodes): {' → '.join(map(str, route))}")
else:
    print("No solution found.")