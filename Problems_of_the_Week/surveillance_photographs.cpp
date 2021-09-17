#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;
typedef std::queue<int> Queue;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;


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


void BFS(const graph& G, int s, MI& policemen) {
  int n = boost::num_vertices(G);
  VI visited(n, 0);
  visited[s] = 1;
  Queue q;
  q.push(s);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    out_edge_it it, end;
    for (boost::tie(it, end) = boost::out_edges(u, G); it != end; it++) {
      if (boost::get(boost::edge_capacity, G, *it) == 0) {
        continue;
      }
      int v = boost::target(*it, G);
      if (!visited[v]) {
        q.push(v);
        visited[v] = 1;
      }
    }
  }
  
  policemen.push_back(visited);
}

void testcase() {
  //Read input
  int n, m, k, l;
  std::cin >> n >> m >> k >> l;
  VI stations(k);
  VI photos(l);
  graph G(n);
  edge_adder adder(G);
  
  for (int i = 0; i < k; i++) {
    std::cin >> stations[i];
  }
  for (int i = 0; i < l; i++) {
    std::cin >> photos[i];
  }
  for (int i = 0; i < m; i++) {
    int u, v; std::cin >> u >> v;
    adder.add_edge(u, v, 1);
  }
  
  //Algorithm
  std::sort(stations.begin(), stations.end());
  MI policemen;
  BFS(G, stations[0], policemen);
  int prev = stations[0];
  for (int s = 1; s < k; s++) {
    if (prev != stations[s]) {
      BFS(G, stations[s], policemen);
    }
    else {
      policemen.push_back(policemen.back());
    }
    prev = stations[s];
  }
  
  VI photosAtNode(n, 0);
  for (int photo : photos) {
    photosAtNode[photo]++;
  }
  for (int i = 0; i < n; i++) {
    int collectionNode = boost::add_vertex(G);
    adder.add_edge(collectionNode, i, photosAtNode[i]);
  }
  
  
  int source = boost::add_vertex(G);
  int sink = boost::add_vertex(G);
  for (int i = 0; i < k; i++) {
    int cop = boost::add_vertex(G);
    adder.add_edge(source, cop, 1);
    for (int j = 0; j < l; j++) {
      if (policemen[i][photos[j]]) {
        adder.add_edge(cop, n+photos[j], 1);
      }
    }
  }
  for (int s : stations) {
    adder.add_edge(s, sink, 1);
  }
  
  long flow = boost::push_relabel_max_flow(G, source, sink);
  std::cout << flow << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
