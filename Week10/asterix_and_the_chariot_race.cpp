#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;
typedef boost::adjacency_list<boost::vecS, boost:: vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

int gen(int i, const VI& costs, const graph& G, const VI& father, MI& memo);
int incl(int i, const VI& costs, const graph& G, const VI& father, MI& memo);

int gen(int i, const VI& costs, const graph& G, const VI& father, MI& memo) {
  if (memo[i][0] != -1) return memo[i][0];
  
  int val1 = incl(i, costs, G, father, memo);
  
  int val2 = std::numeric_limits<int>::max();
  out_edge_it it, end;
  bool found = false;
  int prevJ;
  int sum;
  for (boost::tie(it, end) = boost::out_edges(i, G); it != end; it++) {
    int j = boost::target(*it, G);
    if (j == father[i]) continue;
    if (!found) {
      sum = incl(j, costs, G, father, memo);
      out_edge_it it2, end2;
      for (boost::tie(it2, end2) = boost::out_edges(i, G); it2 != end2; it2++) {
        int j2 = boost::target(*it2, G);
        if (j2 == father[i] or j2 == j) continue;
        sum += gen(j2, costs, G, father, memo);
      }
      val2 = std::min(val2, sum);
      found = true;
      prevJ = j;
    } else {
      sum -= incl(prevJ, costs, G, father, memo);
      sum += gen(prevJ, costs, G, father, memo);
      sum -= gen(j, costs, G, father, memo);
      sum += incl(j, costs, G, father, memo);
      prevJ = j;
      val2 = std::min(val2, sum);
    }
    
  }
  
  int result = std::min(val1, val2);
  memo[i][0] = result;
  return result;
}

int incl(int i, const VI& costs, const graph& G, const VI& father, MI& memo) {
  if (memo[i][1] != -1) return memo[i][1];
  
  int result = costs[i];
  out_edge_it it, end;
  for (boost::tie(it, end) = boost::out_edges(i, G); it != end; it++) {
    int j = boost::target(*it, G);
    if (j == father[i]) continue;
    int grandChildrenValue = 0;
    out_edge_it it2, end2;
    for (boost::tie(it2, end2) = boost::out_edges(j, G); it2 != end2; it2++) {
      int k = boost::target(*it2, G);
      if (k == i) continue;
      grandChildrenValue += gen(k, costs, G, father, memo);
    }
    result += std::min(grandChildrenValue, gen(j, costs, G, father, memo));
  }
  
  memo[i][1] = result;
  return result;
}

void assignFathers(const graph& G, VI& father) {
  std::queue<int> Q;
  int n = boost::num_vertices(G);
  VI visited(n, 0);
  visited[0] = 1;
  Q.push(0);
  father[0] = -1;
  
  while (!Q.empty()) {
    int u = Q.front();
    Q.pop();
    out_edge_it it, end;
    for (boost::tie(it, end) = boost::out_edges(u, G); it != end; it++) {
      int v = boost::target(*it, G);
      if (!visited[v]) {
        visited[v] = 1;
        Q.push(v);
        father[v] = u;
      }
    }
  }
}


void testcase() {
  // Read input
  int n; std::cin >> n;
  graph G(n);
  VI costs(n);
  for (int i = 0; i < n-1; i++) {
    int u, v; std::cin >> u >> v;
    boost::add_edge(u, v, G);
  }
  for (int i = 0; i < n; i++) {
    int c; std::cin >> c;
    costs[i] = c;
  }
  
  //Algorithm
  VI father(n);
  assignFathers(G, father);
  
  MI memo(n, VI(2, -1));
  int result = gen(0, costs, G, father, memo);
  
  //Print solution!
  std::cout << result << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
