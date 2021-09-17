#include <iostream>
#include <vector>
#include <limits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
              boost::property<boost::edge_weight_t, long>>>>> graph;

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
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

typedef std::vector<int> VI;


bool save(int a, int s, int t, int c, int d, const VI& agents, const VI& shelters, const std::vector<VI>& dist_map) {
  graph G(a+c*s);
  edge_adder adder(G);
  int source = boost::add_vertex(G);
  int sink = boost::add_vertex(G);
  
  for (int i = 0; i < a; i++) {
    for (int j = 0; j < s; j++) {
      for (int k = 0; k < c; k++) {
        if (dist_map[i][shelters[j]] <= t - k*d) {
          adder.add_edge(i, j+a+k*s, 1);
        }
      }
    }
    adder.add_edge(source, i, 1);
  }
  for (int i = 0; i < s; i++) {
    for (int k = 0; k < c; k++) {
      adder.add_edge(i+a+k*s, sink, 1);
    }
  }
  
  int flow = boost::push_relabel_max_flow(G, source, sink);
  
  return a == flow;
}

void testcase() {
  // Read input
  int n, m, a, s, c, d;
  std::cin >> n >> m >> a >> s >> c >> d;
  
  graph G(n);
  for (int i = 0; i < m; i++) {
    char w;
    int x, y, z;
    std::cin >> w >> x >> y >> z;
    auto e = boost::add_edge(x, y, G).first;
    boost::put(boost::edge_weight, G, e, z);
    if (w == 'L') {
    e = boost::add_edge(y, x, G).first;
    boost::put(boost::edge_weight, G, e, z);
    }
  }
  
  VI agents(a);
  VI shelters(s);
  for (int i = 0; i < a; i++) {
    std::cin >> agents[i];
  }
  for (int i = 0; i < s; i++) {
    std::cin >> shelters[i];
  }
  
  // Algorithm
  std::vector<VI> dist_map(a, VI(n));
  for (int i = 0; i < a; i++) {
    boost::dijkstra_shortest_paths(G, agents[i],
    boost::distance_map(boost::make_iterator_property_map(
      dist_map[i].begin(), boost::get(boost::vertex_index, G))));
  }
  
  int start = 0;
  int end = std::numeric_limits<int>::max();
  while (start < end) {
    int m = (start+end)/2;
    if (save(a, s, m, c, d, agents, shelters, dist_map)) {
      end = m;
    } else {
      start = m+1;
    }
  }
  
  // Print solution!
  std::cout << start+d << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
