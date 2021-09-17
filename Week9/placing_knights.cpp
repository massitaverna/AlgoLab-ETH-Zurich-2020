#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor > > > > graph;

typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;


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
  int n; std::cin >> n;
  graph G(n*n);
  vertex_desc source = boost::add_vertex(G);
  vertex_desc sink = boost::add_vertex(G);
  edge_adder adder(G);

  bool flag = true;
  bool odd = (n%2 == 1);
  int holes = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (odd or j != 0) {
        flag = !flag;
      }

      int b; std::cin >> b;
      if (b == 0) {
        holes++;
        continue;
      }

      if (!flag) {
        if (i-2 >= 0 and j+1 <= n-1)
        adder.add_edge(i*n+j, (i-2)*n+(j+1), 1);
        if (i-1 >= 0 and j+2 <= n-1)
        adder.add_edge(i*n+j, (i-1)*n+(j+2), 1);
        if (i+1 <= n-1 and j+2 <= n-1)
        adder.add_edge(i*n+j, (i+1)*n+(j+2), 1);
        if (i+2 <= n-1 and j+1 <= n-1)
        adder.add_edge(i*n+j, (i+2)*n+(j+1), 1);
        adder.add_edge(source, i*n+j, 1);
      } else {
        if (i-2 >= 0 and j+1 <= n-1)
        adder.add_edge((i-2)*n+(j+1), i*n+j, 1);
        if (i-1 >= 0 and j+2 <= n-1)
        adder.add_edge((i-1)*n+(j+2), i*n+j, 1);
        if (i+1 <= n-1 and j+2 <= n-1)
        adder.add_edge((i+1)*n+(j+2), i*n+j, 1);
        if (i+2 <= n-1 and j+1 <= n-1)
        adder.add_edge((i+2)*n+(j+1), i*n+j, 1);
        adder.add_edge(i*n+j, sink, 1);
      }
    }
  }

  long flow = boost::push_relabel_max_flow(G, source, sink);
  long maxIS_cardinality = n*n - holes - flow;

  std::cout << maxIS_cardinality << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  while (t--) testcase();
}

