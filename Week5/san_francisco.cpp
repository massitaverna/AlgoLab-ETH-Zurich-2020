#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;
typedef std::vector<long> VL;
typedef std::vector<VL> ML;

struct Node {
  int index;

  Node(int i) : index(i) {}
};

struct Edge {
  int target;
  int weight;

  Edge(int t, int w) : target(t), weight(w) {}
};

typedef std::vector<std::vector<Edge>> AdjacencyList;


long f(int u, int k, const AdjacencyList& adjList, ML& memo) {
  if (k==0) return 0;
  if (memo[u][k] != -1) return memo[u][k];
  
  long max = -1;
  bool oneNeighbour = false;
  for (const Edge& e : adjList[u]) {
    oneNeighbour = true;
    max = std::max(max, e.weight + f(e.target, k-1, adjList, memo));
  }
  if (!oneNeighbour) {
    max = f(0, k, adjList, memo);
  }

  memo[u][k] = max;
  return max;
}

void testcase() {
  //Read input
  int n, m; long x; int k; std::cin >> n >> m >> x >> k;

  std::vector<Node> nodes;
  nodes.reserve(n);
  for (int i = 0; i < n; i++) {
    nodes.push_back(Node(i));
  }

  AdjacencyList adjList(n, std::vector<Edge>());
  MI ADJS (n, VI(n,-1));
  for (int i = 0; i < m; i++) {
    int u, v, p; std::cin >> u >> v >> p;
    if (ADJS[u][v] < p) {
      ADJS[u][v] = p;
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (ADJS[i][j] != -1) {
        adjList[i].push_back(Edge(j, ADJS[i][j]));
      }
    }
  }
  
  //Algorithm
  ML memo(n, VL(k+1, -1));
  if (f(0, k, adjList, memo) < x) std::cout << "Impossible" << std::endl;
  else {
    for (int i = 0; i <= k; i++) {
      if (f(0, i, adjList, memo) >= x) {
        std::cout << i << std::endl;
        return;
      }
    }
  }
}


int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }

  return 0;
}
