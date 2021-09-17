#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long>>>>> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};


void testcase() {
  // Read input
  int n, m, budget, k, a;
  std::cin >> n >> m >> budget >> k >> a;
  
  graph G(n);
  edge_adder adder(G);
  
  for (int i = 0; i < m; i++) {
    int x, y, d, e; std::cin >> x >> y >> d >> e;
    adder.add_edge(x, y, e, d);
  }
  
  // Algorithm
  int source = boost::add_vertex(G);
  adder.add_edge(source, k, 0, 0);
  
  int start = 0;
  int end = boost::push_relabel_max_flow(G, k, a);
  edge_desc source_edge = boost::edge(source, k, G).first;
  
  while (start < end) {
    int mid = (start+end+1)/2;
    boost::put(boost::edge_capacity, G, source_edge, mid);
    boost::successive_shortest_path_nonnegative_weights(G, source, a);
    int cost = boost::find_flow_cost(G);
    if (cost > budget) end = mid-1;
    else start = mid;
  }
  
  // Print solution!
  std::cout << start << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
