#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator outedge_it;

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
  int m, n, k, c; std::cin >> m >> n >> k >> c;
  graph G(2*m*n);
  vertex_desc source = boost::add_vertex(G);
  vertex_desc sink = boost::add_vertex(G);
  edge_adder adder(G);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (j < m-1) {
        adder.add_edge(i*m+j+m*n, i*m+(j+1), 1);
        adder.add_edge(i*m+(j+1)+m*n, i*m+j, 1);
      }
      if (i < n-1) {
        adder.add_edge(i*m+j+m*n, (i+1)*m+j, 1);
        adder.add_edge((i+1)*m+j+m*n, i*m+j, 1);
      }

      int boundary = 0;
      if (i == 0) boundary++;
      if (j == 0) boundary++;
      if (i == n-1) boundary++;
      if (j == m-1) boundary++;
      if (boundary) {
        adder.add_edge(i*m+j+m*n, sink, boundary);
      }

      adder.add_edge(i*m+j, i*m+j+m*n, c);
    }
  }

  for (int i = 0; i < k; i++) {
    int x, y; std::cin >> x >> y;
    int v = y*m + x;
    adder.add_edge(source, v, 1);
  }

  int flow = boost::push_relabel_max_flow(G, source, sink);
  std::cout << flow << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }

  return 0;
}
