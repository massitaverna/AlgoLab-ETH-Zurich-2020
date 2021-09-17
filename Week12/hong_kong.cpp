#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>


typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 Point;
typedef std::vector<Point> VP;

void testcase() {
  // Read input
  int n, m;
  double r;
  std::cin >> n >> m >> r;
  VP trees;
  trees.reserve(n);
  VP baloons;
  baloons.reserve(m);
  std::vector<double> radii(m);
  
  for (int i = 0; i < n; i++) {
    double x, y; std::cin >> x >> y;
    trees.push_back(Point(x, y));
  }
  for (int i = 0; i < m; i++) {
    double x, y, s; std::cin >> x >> y >> s;
    baloons.push_back(Point(x, y));
    radii[i] = s;
  }
  
  // Algorithm
  Triangulation t;
  t.insert(trees.begin(), trees.end());
  
  const K::FT INF = std::pow(2, 200);
  std::priority_queue<std::pair<K::FT, Face_handle>> Q;
  
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); f++) {
    if (t.is_infinite(f)) {
      f->info() = INF;
    }
    else {
      Point p = t.dual(f);
      Point v = f->vertex(0)->point();
      f->info() = CGAL::squared_distance(p, v);
    }
    Q.push({f->info(), f});
  }
  
  while (!Q.empty()) {
    Face_handle f = Q.top().second;
    K::FT escape_dist = Q.top().first;
    Q.pop();
    if (escape_dist != f->info()) continue;
    for (int i = 0; i < 3; i++) {
      if (t.is_infinite(f->neighbor(i))) continue;
      K::FT edge_length = t.segment({f, i}).squared_length();
      K::FT new_dist = std::min(edge_length, f->info());
      if (f->neighbor(i)->info() < new_dist) {
        f->neighbor(i)->info() = new_dist;
        Q.push({new_dist, f->neighbor(i)});
      }
    }
  }
  
  // Find solutions!
  for (int i = 0; i < m; i++) {
    Point b = baloons[i];
    Vertex_handle nearest = t.nearest_vertex(baloons[i]);
    K::FT sqR = K::FT(radii[i] + r);
    sqR *= sqR;
    Face_handle f = t.locate(b);
    if (CGAL::squared_distance(b, nearest->point()) >= sqR and
        4*sqR <= f->info()) {
      std::cout << 'y';
    } else {
      std::cout << 'n';
    }
  }
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}

