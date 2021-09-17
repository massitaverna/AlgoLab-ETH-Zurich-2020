#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define MAX_PR 128

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
  int n, m, l; std::cin >> n >> m >> l;
  graph G(n+1);
  int source = n;
  int sink = n-1;
  edge_adder adder(G);

  adder.add_edge(source, 0, l, 0);
  for (int i = 0; i < n-1; i++) {
    adder.add_edge(i, i+1, l, MAX_PR);
  }
  for (int i = 0; i < m; i++) {
    int x, y, p; std::cin >> x >> y >> p;
    adder.add_edge(x, y, 1, MAX_PR*(y-x) - p);
  }

  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int cost = boost::find_flow_cost(G);
  int priority = -cost + MAX_PR*(n-1)*l;

  std::cout << priority << std::endl;

}


int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  while (t--) testcase();
}
