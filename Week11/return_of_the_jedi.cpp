#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#define MAX_VALUE 0x100000 + 1

struct Edge {
  int u;
  int v;
  int w;
  bool operator < (const Edge& other) const {
    return w > other.w;
  }
  Edge(int a, int b, int c) : u(a), v(b), w(c) {}
};

typedef std::vector<int> VI;
typedef std::vector<VI> MI;
typedef std::vector<bool> VB;
typedef std::vector<VB> MB;
typedef std::priority_queue<Edge> PriorityQueue;


void testcase() {
  // Read input
  int n, garbage;
  std::cin >> n >> garbage;
  MI weights(n, VI(n));
  
  for (int i = 0; i < n-1; i++) {
    for (int j = i+1; j < n; j++) {
      std::cin >> weights[i][j];
      weights[j][i] = weights[i][j];
    }
  }
  
  // Algorithm
  PriorityQueue Q;
  VB isCovered(n, false);
  MB isEdgePicked(n, VB(n, false));
  MI biggestEdgeInPath(n, VI(n));
  int cost = 0;
  
  for (int i = 1; i < n; i++) {
    Q.push(Edge(0, i, weights[0][i]));
  }
  isCovered[0] = true;
  
  while (!Q.empty()) {
    Edge e = Q.top();
    Q.pop();
    int u = e.u;
    int v = e.v;
    if (isCovered[v]) continue;
    isEdgePicked[u][v] = true;
    isEdgePicked[v][u] = true;
    cost += weights[u][v];
    biggestEdgeInPath[u][v] = weights[u][v];
    biggestEdgeInPath[v][u] = weights[u][v];
    isCovered[v] = true;
    for (int vertex = 0; vertex < n; vertex++) {
      if (isCovered[vertex] and vertex != u and vertex != v) {
        biggestEdgeInPath[vertex][v] = std::max(biggestEdgeInPath[vertex][u], weights[u][v]);
        biggestEdgeInPath[v][vertex] = biggestEdgeInPath[vertex][v];
      }
      if (!isCovered[vertex]) {
        Q.push(Edge(v, vertex, weights[v][vertex]));
      }
    }
  }
  
  int costToAdd = MAX_VALUE;
  for (int u = 0; u < n-1; u++) {
    for (int v = u+1; v < n; v++) {
      if (!isEdgePicked[u][v]) {
        costToAdd = std::min(costToAdd, weights[u][v] - biggestEdgeInPath[u][v]);
      }
    }
  }
  
  // Print solution!
  std::cout << cost + costToAdd << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}

