#include <iostream>
#include <vector>
#include <set>
#include <boost/graph/adjacency_list.hpp>

typedef std::vector<int> VI;
typedef std::set<int> Set;
typedef std::multiset<int> Multiset;
typedef boost::adjacency_list<boost::vecS, boost:: vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;


void DFS(graph& G, int u, VI& path, Multiset& temps, size_t m, int k, const VI& H, Set& result) {
  path.push_back(u);
  temps.insert(H[u]);

  if (path.size() > m) {
    temps.erase(temps.find(H[path[path.size() - m - 1]]));
  }
  if (path.size() >= m) {
    int min = *(temps.begin());
    int max = *(temps.rbegin());
    if (max-min <= k) {
      result.insert(path[path.size() - m]);
    }
  }
  
  out_edge_it it, end;
  for (boost::tie(it, end) = boost::out_edges(u, G); it != end; it++) {
    int v = boost::target(*it, G);
    DFS(G, v, path, temps, m, k, H, result);
  }
  
  path.pop_back();
  temps.erase(temps.find(H[u]));
  if (path.size() >= m) {
    temps.insert(H[path[path.size() - m]]);
  }
}

void testcase() {
  // Read input
  int n, m, k; std::cin >> n >> m >> k;
  graph G(n);
  VI H(n);
  for (int i = 0; i < n; i++) {
    std::cin >> H[i];
  }
  for (int i = 0; i < n-1; i++) {
    int u, v; std::cin >> u >> v;
    boost::add_edge(u, v, G);
  }
  
  //Algorithm
  VI path;
  path.reserve(n);
  Multiset temps;
  Set result;
  
  DFS(G, 0, path, temps, m, k, H, result);
  
  //Print solution!
  for (int r : result) {
    std::cout << r << " ";
  }
  if (result.empty()) {
    std::cout << "Abort mission";
  }
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
