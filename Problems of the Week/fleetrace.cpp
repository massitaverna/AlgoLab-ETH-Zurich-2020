#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define MAX_COST 50

typedef std::vector<int> VI;
typedef std::vector<VI> MI;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long>>>>> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;


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
  int b, s, p; std::cin >> b >> s >> p;
  graph G(b+s);
  edge_adder adder(G);
  
  for (int i = 0; i < p; i++) {
    int x, y, c; std::cin >> x >> y >> c;
    adder.add_edge(x, y+b, 1, MAX_COST - c);
  }
  
  int source = boost::add_vertex(G);
  int target = boost::add_vertex(G);
  for (int i = 0; i < b; i++) {
    adder.add_edge(source, i, 1, 0);
    adder.add_edge(i, target, 1, MAX_COST);
  }
  for (int i = 0; i < s; i++) {
    adder.add_edge(i+b, target, 1, 0);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, source, target);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(source, G); e != eend; ++e) {
    s_flow += c_map[*e] - rc_map[*e];     
  }
  int cost = boost::find_flow_cost(G);
  cost = -cost + s_flow*MAX_COST;
  
  std::cout << cost << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
