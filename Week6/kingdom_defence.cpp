#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef std::vector<int> VI;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
boost::property<boost::edge_capacity_t, long,
boost::property<boost::edge_residual_capacity_t, long,
boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;

  }
};


void testcase() {
  int l, p; std::cin >> l >> p;
  graph G(l);
  VI demands(l,0);

  for (int i = 0; i < l; i++) {
    int g, d; std::cin >> g >> d;
    demands[i] = d-g;
  }

  edge_adder adder(G);
  for (int i = 0; i < p; i++) {
    int f, t, min, max; std::cin >> f >> t >> min >> max;
    if (f == t) continue;
    adder.add_edge(f, t, max - min);
    demands[f] += min;
    demands[t] -= min;
    
  }

  vertex_desc source = boost::add_vertex(G);
  vertex_desc sink = boost::add_vertex(G);
  long req_flow = 0;
  for (int i = 0; i < l; i++) {
    if (demands[i] > 0) {
      adder.add_edge(i, sink, demands[i]);
      req_flow += demands[i];
    }
    else if (demands[i] < 0) {
      adder.add_edge(source, i, -demands[i]);
    }
  }

  long max_flow = boost::push_relabel_max_flow(G, source, sink);
  
  if (max_flow == req_flow) {
    std::cout << "yes" << std::endl;
  }
  else {
    std::cout << "no"  << std::endl;
  }
  
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  while (t--) {
    testcase();
  }
}
