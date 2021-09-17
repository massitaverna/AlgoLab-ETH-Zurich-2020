#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Vertex_handle Vh;
typedef K::Point_2 Point;
typedef std::vector<Point> VP;


int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int n;
  std::cin >> n;

  while (n != 0) {
    VP existing(n);
    for (int i = 0; i < n; i++) {
      int x, y; std::cin >> x >> y;
      existing[i] = Point(x, y);
    }

    int m; std::cin >> m;
    VP news(m);
    for (int i = 0; i < m; i++) {
      int x, y; std::cin >> x >> y;
      news[i] = Point(x, y);
    }

    Triangulation t;
    t.insert(existing.begin(), existing.end());

    for (const Point& p : news) {
      Vh vh = t.nearest_vertex(p);
      double distance = CGAL::squared_distance(p, vh->point());
      std::cout << distance << std::endl;
    }

    std::cin >> n;
  }

  return 0;
}
