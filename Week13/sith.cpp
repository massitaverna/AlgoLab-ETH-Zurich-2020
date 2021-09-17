#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef std::vector<int> VI;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::Point_2 Point;
typedef std::vector<std::pair<Point, int>> VP;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;


void testcase() {
  int n;
  double sqR;
  std::cin >> n >> sqR;
  sqR *= sqR;
  VP points(n);
  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    points[i] = {Point(x, y), i};
  }
  
  int start = 1;
  int end = n/2+1;

  while (start < end) {
    int m = (start+end+1)/2;
    auto it = points.begin();
    for (int i = 0; i < m; i++) {
      it++;
    }
    Triangulation t;
    t.insert(it, points.end());
    
    graph G;
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
      if (t.segment(e).squared_length() <= sqR) {
        int v1 = e->first->vertex((e->second+1)%3)->info() - m;
        int v2 = e->first->vertex((e->second+2)%3)->info() - m;
        boost::add_edge(v1, v2, G);
      }
    }
    
    int num_vertices = boost::num_vertices(G);
    VI component_map(num_vertices);
    int ncc = boost::connected_components(G, &component_map[0]); 
    VI nodesInCC(ncc, 0);
    bool res = false;
    for (int i = 0; i < num_vertices; i++) {
      nodesInCC[component_map[i]]++;
      if (nodesInCC[component_map[i]] >= m) res = true;
    }
    
    if (res) {
      start = m;
    } else {
      end = m-1;
    }
  }
  
  std::cout << start << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
