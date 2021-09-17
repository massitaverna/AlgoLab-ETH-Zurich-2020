#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

typedef std::string String;
typedef std::vector<int> VI;
typedef std::vector<VI> MI;
typedef std::pair<int, int> Query;
typedef std::vector<std::vector<Query>> MQ; // Matrix of Queries
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;


int solve(const Query& query, const VI& path, const VI& ages) {
  int start = 0;
  int end = path.size() - 1;
  while (start < end) {
    int mid = (start+end)/2;
    if (ages[path[mid]] <= query.first) end = mid;
    else start = mid+1;
  } return path[start];
}

// DFS Complexity: O(n + q*log(n))
void DFS(const graph& G, int u, VI& path, const MQ& queriesPerSpecies, const VI& ages, VI& results) {
  path.push_back(u);
  for (const Query& query : queriesPerSpecies[u]) {
    int result = solve(query, path, ages);
    results[query.second] = result;
  }
  out_edge_it it, end;
  for (boost::tie(it, end) = boost::out_edges(u, G); it != end; it++) {
    int v = boost::target(*it, G);
    DFS(G, v, path, queriesPerSpecies, ages, results);
  }
  path.pop_back();
}

// Testcase complexity: O(n + q*log(n))
void testcase() {
  int n, q; std::cin >> n >> q;
  
  std::unordered_map<String, int> IDs;
  std::unordered_map<int, String> names;
  VI ages(n);
  int max_age = -1;
  int root = -1;
  for (int i = 0; i < n; i++) {
    String s; std::cin >> s;
    std::cin >> ages[i];
    IDs.insert({s, i});
    names.insert({i, s});
    if (ages[i] > max_age) {
      max_age = ages[i];
      root = i;
    }
  }
  
  graph G(n);
  for (int i = 0; i < n-1; i++) {
    String s, p; std::cin >> s >> p;
    int id_s = IDs.at(s);
    int id_p = IDs.at(p);
    boost::add_edge(id_p, id_s, G);
  }

  // species --> vector of {b, query_index}
  MQ queriesPerSpecies(n);
  for (int i = 0; i < q; i++) {
    String s; std::cin >> s;
    int b; std::cin >> b;
    int id_s = IDs.at(s);
    queriesPerSpecies[id_s].push_back({b, i});
  }
  
  VI path;
  VI results(q);
  DFS(G, root, path, queriesPerSpecies, ages, results);
  
  // Print solution!
  for (int result : results) {
    std::cout << names.at(result) << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
  
}
