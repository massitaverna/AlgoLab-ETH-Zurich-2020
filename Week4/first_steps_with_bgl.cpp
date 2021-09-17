#include <iostream>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::undirectedS,
        boost::no_property, boost::property<boost::edge_weight_t, int>
            > weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator edge_it;


void testcase() {
  //Read input
  int n; std::cin >> n;
  int m; std::cin >> m;
  weighted_graph G(n);

  for (int i = 0; i < m; i++) {
    int u; std::cin >> u;
    int v; std::cin >> v;
    edge_desc e = (boost::add_edge(u, v, G)).first;
    int w; std::cin >> w;
    put(boost::edge_weight, G, e, w);
  }


  //Find Minimum Spanning Tree
  std::vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  assert(mst.size() == n-1);

  //Compute its weight
  int weight = 0;

  edge_it it, end;
  boost::tie(it, end) = boost::edges(G);
  for (edge_desc ed : mst) {
    weight += boost::get(boost::edge_weight, G, ed);
  }

  //Find distances from vertex 0
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, 0, boost::distance_map(boost::make_iterator_property_map(
          dist_map.begin(), boost::get(boost::vertex_index, G))));
  //Find distance from furthest vertex
  int distance = *std::max_element(dist_map.begin(), dist_map.end());

  //Print solution
  std::cout << weight << " " << distance << std::endl;
  
  
}


int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }

  return 0;
}
