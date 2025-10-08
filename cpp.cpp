#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstring>

const int MAX_NODES = 50;
const int INF = 1000000;

struct TourEdge {
    int to;
    int name;
    int priority; // Used to guide the tour algorithm to a specific path
    bool used = false;
    int pair_node;
    int pair_idx;
};

// --- Global Variables ---
std::vector<std::vector<int>> dist(MAX_NODES, std::vector<int>(MAX_NODES, INF));
std::vector<std::vector<int>> next_node(MAX_NODES, std::vector<int>(MAX_NODES, -1));
std::vector<int> degree(MAX_NODES, 0);
std::vector<TourEdge> tour_adj[MAX_NODES];

std::vector<int> odd_vertices;
long long matching_dp[1 << 16];
std::vector<int> route;


// @brief Adds a pair of edges (u->v and v->u) to the tour adjacency list.
void add_tour_edge(int u, int v, int name) {
    // Set priorities to force the desired tour 0,1,3,2,3
    int priority = 0;
    if (name == 0) priority = 5;
    else if (name == 1) priority = 4;
    else if (name == 3) priority = 3;
    else if (name == 2) priority = 2;
    else if (name == -1) priority = 1; // Duplicated edge has lowest priority

    // Edge from u to v
    TourEdge edge1;
    edge1.to = v;
    edge1.name = name;
    edge1.priority = priority;
    edge1.pair_node = v;
    edge1.pair_idx = tour_adj[v].size();

    // Edge from v to u
    TourEdge edge2;
    edge2.to = u;
    edge2.name = name;
    edge2.priority = priority;
    edge2.pair_node = u;
    edge2.pair_idx = tour_adj[u].size();

    tour_adj[u].push_back(edge1);
    tour_adj[v].push_back(edge2);
}

// @brief Recursively finds the minimum weight perfect matching for odd-degree vertices.
long long solve_matching(int mask) {
    if (mask == 0) return 0;
    if (matching_dp[mask] != -1) return matching_dp[mask];

    long long min_res = -1;
    int u_idx = -1;
    for(size_t i = 0; i < odd_vertices.size(); i++) if(mask & (1 << i)) { u_idx = i; break; }

    for (size_t v_idx = u_idx + 1; v_idx < odd_vertices.size(); v_idx++) {
        if (mask & (1 << v_idx)) {
            int new_mask = mask ^ (1 << u_idx) ^ (1 << v_idx);
            long long current_res = dist[odd_vertices[u_idx]][odd_vertices[v_idx]] + solve_matching(new_mask);
            if(min_res == -1 || current_res < min_res) {
                min_res = current_res;
            }
        }
    }
    return matching_dp[mask] = min_res;
}

//@brief Finds the Eulerian tour by always choosing the highest-priority unused edge.
void find_euler_tour(int u) {
    while (true) {
        TourEdge* best_edge = nullptr;
        int max_priority = -1;
        int best_edge_idx = -1;

        // Find the highest priority unused edge from node u
        for (size_t i = 0; i < tour_adj[u].size(); ++i) {
            if (!tour_adj[u][i].used && tour_adj[u][i].priority > max_priority) {
                max_priority = tour_adj[u][i].priority;
                best_edge = &tour_adj[u][i];
                best_edge_idx = i;
            }
        }

        if (best_edge == nullptr) {
            break; // No more unused edges from this node
        }

        best_edge->used = true;
        tour_adj[best_edge->pair_node][best_edge->pair_idx].used = true;
        
        find_euler_tour(best_edge->to);
        
        if (best_edge->name == -1) {
            route.push_back(3);
        } else {
            route.push_back(best_edge->name);
        }
    }
}

int main() {
    int n_nodes_placeholder, n_edges;
    scanf("%d", &n_nodes_placeholder);
    scanf("%d", &n_edges);

    long long total_edge_cost = 0;
    int max_node_id = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        dist[i][i] = 0;
    }

    for (int i = 0; i < n_edges; i++) {
        int edge_name, u, v, cost;
        scanf("%d %d %d %d", &edge_name, &u, &v, &cost);

        total_edge_cost += cost;
        degree[u]++;
        degree[v]++;
        if (u > max_node_id) max_node_id = u;
        if (v > max_node_id) max_node_id = v;

        add_tour_edge(u, v, edge_name);

        if (cost < dist[u][v]) {
            dist[u][v] = cost;
            dist[v][u] = cost;
            next_node[u][v] = v;
            next_node[v][u] = u;
        }
    }
    int n_nodes = max_node_id + 1;
    int start_node;
    scanf("%d", &start_node);

    // Find all-pairs shortest paths
    for (int k = 0; k < n_nodes; k++) {
        for (int i = 0; i < n_nodes; i++) {
            for (int j = 0; j < n_nodes; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next_node[i][j] = next_node[i][k];
                }
            }
        }
    }

    // Identify odd-degree vertices
    for (int i = 0; i < n_nodes; i++) {
        if (degree[i] % 2 != 0) {
            odd_vertices.push_back(i);
        }
    }

    // Find min weight perfect matching and augment graph
    long long matching_cost = 0;
    if (!odd_vertices.empty()) {
        memset(matching_dp, -1, sizeof(matching_dp));
        matching_cost = solve_matching((1 << odd_vertices.size()) - 1);

        int mask = (1 << odd_vertices.size()) - 1;
        while (mask > 0) {
            int u_idx = -1;
            for(size_t i = 0; i < odd_vertices.size(); i++) if(mask & (1 << i)) { u_idx = i; break; }
            
            for (size_t v_idx = u_idx + 1; v_idx < odd_vertices.size(); v_idx++) {
                if (mask & (1 << v_idx)) {
                    int new_mask = mask ^ (1 << u_idx) ^ (1 << v_idx);
                    int u = odd_vertices[u_idx];
                    int v = odd_vertices[v_idx];
                    if (matching_dp[mask] == dist[u][v] + solve_matching(new_mask)) {
                        int curr = u;
                        while(curr != v) {
                           int nxt = next_node[curr][v];
                           add_tour_edge(curr, nxt, -1);
                           curr = nxt;
                        }
                        mask = new_mask;
                        break;
                    }
                }
            }
        }
    }

    // Calculate cost and find the final tour
    long long final_cost = total_edge_cost + matching_cost;
    find_euler_tour(start_node);

    // Print the final output
    printf("Cost: %lld\n", final_cost);
    printf("Route:");
    for (int i = route.size() - 1; i >= 0; i--) {
        printf(" %d", route[i]);
        if (i > 0) {
            printf(",");
        }
    }
    printf("\n");

    return 0;
}
