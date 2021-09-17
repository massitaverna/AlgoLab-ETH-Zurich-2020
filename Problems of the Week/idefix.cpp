#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/pending/disjoint_sets.hpp>


typedef std::vector<int> VI;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 Point;
typedef std::vector<Point> VP;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;


//Comparion functions for Part II
bool nearestTreeCompare(const std::tuple<int, K::FT>& t1, const std::tuple<int, K::FT>& t2) {
  return std::get<1>(t1) < std::get<1>(t2);
}

bool edgesCompare(const std::tuple<int, int, K::FT>& e1,
  const std::tuple<int, int, K::FT>& e2) {
  return std::get<2>(e1) < std::get<2>(e2);
}


//Returns the number of CCs and for each bone finds the CC to which it belongs
int boneToCC_assignment(const Triangulation& t, int n, double s,
  const std::vector<std::tuple<int, K::FT>>& nearestTree, VI& boneToCC) {
  graph G(n);
  for (auto it = t.finite_edges_begin(); it != t.finite_edges_end(); it++) {
    if (t.segment(it).squared_length() <= s) {
      Triangulation::Vertex_handle v1 = it->first->vertex((it->second + 1) % 3);
      Triangulation::Vertex_handle v2 = it->first->vertex((it->second + 2) % 3);
      boost::add_edge(v1->info(), v2->info(), G);
    }
  }
  VI component_map(n);
  int ncc = boost::connected_components(G, boost::make_iterator_property_map(
    component_map.begin(), boost::get(boost::vertex_index, G)));
  
  for (size_t i = 0; i < nearestTree.size(); i++) {
    if (4*std::get<1>(nearestTree[i]) <= s) {
      boneToCC[i] = component_map[std::get<0>(nearestTree[i])];
    }
    else break;
  }
  
  return ncc;
}


void testcase() {
  // Read input
  int n, m, k;
  double s;
  std::cin >> n >> m >> s >> k;
  VP trees(n);
  VP bones(m);
  
  for (int i = 0; i < n; i++) {
    std::cin >> trees[i];
  }
  for (int i = 0; i < m; i++) {
    std::cin >> bones[i];
  }

  // Algorithm

  // Part I
  Triangulation t;
  t.insert(trees.begin(), trees.end());
  int cnt = 0;
  for (auto it = t.finite_vertices_begin(); it != t.finite_vertices_end(); it++) {
    it->info() = cnt;
    cnt++;
  }

  std::vector<std::tuple<int, K::FT>> nearestTree;
  for (int i = 0; i < m; i++) {
    Vertex_handle v = t.nearest_vertex(bones[i]);
    K::FT dist = CGAL::squared_distance(v->point(), bones[i]);
    int index = v->info();
    nearestTree.emplace_back(index, dist);
  }
  std::sort(nearestTree.begin(), nearestTree.end(), nearestTreeCompare);

  VI boneToCC(m, -1);
  int numCC = boneToCC_assignment(t, n, s, nearestTree, boneToCC);
  VI bonesInCC(numCC, 0);
  for (int cc : boneToCC) {
    if (cc == -1) continue;
    bonesInCC[cc]++;
  }
  
  int a = *(std::max_element(bonesInCC.begin(), bonesInCC.end()));
  
  // Part II
  bonesInCC.clear();
  bonesInCC.reserve(n);
  for (int i = 0; i < n; i++) {
    bonesInCC.push_back(0);
  }

  std::vector<std::tuple<int, int, K::FT>> edges;
  for (auto it = t.finite_edges_begin(); it != t.finite_edges_end(); it++) {
    Vertex_handle v1 = it->first->vertex((it->second+1)%3);
    Vertex_handle v2 = it->first->vertex((it->second+2)%3);
    K::FT dist = CGAL::squared_distance(v1->point(), v2->point());
    edges.emplace_back(v1->info(), v2->info(), dist);
  }
  
  std::sort(edges.begin(), edges.end(), edgesCompare);
  size_t i = 0;
  size_t j = 0;
  K::FT q = 0;
  bool found = false;
  boost::disjoint_sets_with_storage<> uf(n);
  
  while (i < nearestTree.size() and j < edges.size()) {
    int root;
    if (4*std::get<1>(nearestTree[i]) <= std::get<2>(edges[j])) {
      q = std::max(q, 4*std::get<1>(nearestTree[i]));
      root = uf.find_set(std::get<0>(nearestTree[i]));
      i++;
      bonesInCC[root]++;
    }
    else {
      q = std::max(q, std::get<2>(edges[j]));
      root = uf.find_set(std::get<0>(edges[j]));
      int other = uf.find_set(std::get<1>(edges[j]));
      j++;
      if (root == other) continue;
      uf.link(root, other);
      if (root != uf.find_set(root)) std::swap(root, other);
      bonesInCC[root] += bonesInCC[other];
    }
    if (bonesInCC[root] >= k) {
      found = true;
      break;
    }
  }
  while (!found and i < nearestTree.size()) {
    q = std::max(q, 4*std::get<1>(nearestTree[i]));
    int root = uf.find_set(std::get<0>(nearestTree[i]));
    i++;
    bonesInCC[root]++;
    if (bonesInCC[root] >= k) {
      found = true;
      break;
    }
  }
  while (!found and j < edges.size()) {
    q = std::max(q, std::get<2>(edges[j]));
    int root = uf.find_set(std::get<0>(edges[j]));
    int other = uf.find_set(std::get<1>(edges[j]));
    j++;
    if (root == other) continue;
    uf.link(root, other);
    if (root != uf.find_set(root)) std::swap(root, other);
    bonesInCC[root] += bonesInCC[other];
    if (bonesInCC[root] >= k) {
      found = true;
      break;
    }
  }

  // Print solution!
  std::cout << a << " " << q << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
