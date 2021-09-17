#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <queue>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int,
    boost::property<boost::edge_capacity_t, long>>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
            
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};



void testcase() {
  // Read input
  int n, m, source, sink;
  std::cin >> n >> m >> source >> sink;
  weighted_graph G(n);
  MI weights(n, VI(n, -1));
  MI capacities(n, VI(n, -1));

  graph G_flow(n);
  edge_adder adder(G_flow);
  
  std::set<std::pair<int, int>> edges;
  for (int i = 0; i < m; i++) {
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    if (a == b) continue;
    
    if (a > b) std::swap(a, b);
    if (weights[a][b] == -1 or weights[a][b] > d) {
      weights[a][b] = d;
      capacities[a][b] = c;
      capacities[b][a] = c;
      edges.insert({a, b});
    }
    else if (weights[a][b] == d) {
      capacities[a][b] += c;
      capacities[b][a] += c;
    }
  }

  for (auto edge : edges) {
    int a = edge.first;
    int b = edge.second;
    edge_desc e = boost::add_edge(a, b, G).first;
    boost::put(boost::edge_weight, G, e, weights[a][b]);
  }
   
  
  // Algorithm
  VI dist_map(n);

  boost::dijkstra_shortest_paths(G, sink,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  std::queue<vertex_desc> Q;
  VI visited(n, 0);
  visited[source] = 1;
  Q.push(source);
  
  while(!Q.empty()) {
    vertex_desc u = Q.front();
    Q.pop();
    out_edge_it it, end;
    for (boost::tie(it, end) = boost::out_edges(u, G); it != end; it++) {
      vertex_desc v = boost::target(*it, G);
      int w = boost::get(boost::edge_weight, G, *it);
      if (dist_map[v] + w == dist_map[u]) {
        adder.add_edge(u, v, capacities[u][v]);
        if (!visited[v]) {
          visited[v] = 1;
          Q.push(v);
        }
      }
    }
  }
  
  int max_runners = boost::push_relabel_max_flow(G_flow, source, sink);

  //Print solution!
  std::cout << max_runners << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
