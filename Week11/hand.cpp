#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef std::vector<int> VI;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 Point;
typedef std::vector<std::pair<Point, int>> VP;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;


int max_families(double s, const Triangulation& t, int k) {
  int n = t.number_of_vertices();
  graph G(n);
  for (auto it = t.finite_edges_begin(); it != t.finite_edges_end(); it++) {
    if (t.segment(it).squared_length() < s) {
      int u = it->first->vertex((it->second + 1)%3)->info();
      int v = it->first->vertex((it->second + 2)%3)->info();
      boost::add_edge(u, v, G);
    }
  }
  
  VI component_map(n);
  int ncc = boost::connected_components(G, &component_map[0]);
  VI nodesInCC(ncc, 0);
  for (int i = 0; i < n; i++) {
    nodesInCC[component_map[i]]++;
  }
  VI ccPerSize(k+1, 0);
  for (int i = 0; i < ncc; i++) {
    if (nodesInCC[i] >= k) ccPerSize[k]++;
    else ccPerSize[nodesInCC[i]]++;
  }
  
  int result = ccPerSize[k];
  int min;
  switch (k) {
    case 1:
      break;
    case 2:
      result += ccPerSize[1]/2;
      break;
    case 3:
      min = std::min(ccPerSize[1], ccPerSize[2]);
      result += min;
      ccPerSize[1] -= min;
      ccPerSize[2] -= min;
      result += ccPerSize[1]/3;
      result += ccPerSize[2]/2;
      break;
    case 4:
      result += ccPerSize[2]/2;
      ccPerSize[2] = ccPerSize[2]%2;
      min = std::min(ccPerSize[1], ccPerSize[3]);
      result += min;
      ccPerSize[1] -= min;
      ccPerSize[3] -= min;
      result += ccPerSize[3]/2;
      ccPerSize[3] = ccPerSize[3]%2;
      result += ccPerSize[2] and ccPerSize[3];
      result += ccPerSize[1]/4;
      ccPerSize[1] = ccPerSize[1]%4;
      result += ((ccPerSize[1] >= 2) and ccPerSize[2]);
      break;
  }
  return result;
}

void testcase() {
  // Read input
  int n, k, f0;
  double s0;
  std::cin >> n >> k >> f0 >> s0;

  VP pts;
  pts.reserve(n);
  for (int i = 0; i < n; i++) {
    int x, y;
    std::cin >> x >> y;
    pts.emplace_back(Point(x, y), i);
  }
  
  // Algorithm
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  int f = max_families(s0, t, k);

  std::vector<double> edge_lengths;
  edge_lengths.reserve(3*n);
  for (auto it = t.finite_edges_begin(); it != t.finite_edges_end(); it++) {
    edge_lengths.push_back(t.segment(it).squared_length());
  }
  std::sort(edge_lengths.begin(), edge_lengths.end());
  int start = 0;
  int end = (int) edge_lengths.size()-1;
  while (start < end) {
    long m = (start+end)/2;
    int val = max_families(edge_lengths[m], t, k);
    if (val >= f0) start = m+1;
    else end = m;
  }

  std::cout << edge_lengths[start-1] << " " << f << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);
  int t; std::cin >> t;
  while (t--) testcase();
}
