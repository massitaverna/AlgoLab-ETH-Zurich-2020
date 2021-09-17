#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef std::vector<Point> VP;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<long,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Face_handle Face_handle;


void computeEscapeCapacities(Triangulation& t) {
  std::queue<Face_handle> Q;
  for (auto face = t.all_faces_begin(); face != t.all_faces_end(); face++) {
    if (t.is_infinite(face)) {
      face->info() = std::numeric_limits<long>::max();
      Q.push(face);
    } else {
      face->info() = 0;
    }
  }

  while (!Q.empty()) {
    Face_handle curr = Q.front();
    Q.pop();
    for (int i = 0; i < 3; i++) {
      if (t.is_infinite(curr->neighbor(i))) {
        continue;
      }

      long dist = CGAL::squared_distance(curr->vertex((i+1)%3)->point(),
                 curr->vertex((i+2)%3)->point());
      long newCapacity = std::min(curr->info(), dist);
      if (newCapacity > curr->neighbor(i)->info()) {
        curr->neighbor(i)->info() = newCapacity;
        Q.push(curr->neighbor(i));
      }
    }
  }
}


// Time Complexity: O(n*logn + m*logn)
void testcase(int n) {

  VP infected;
  infected.reserve(n);
  for (int i = 0; i < n; i++) {
    Point p; std::cin >> p;
    infected.push_back(p);
  }

  Triangulation t;
  t.insert(infected.begin(), infected.end());

  computeEscapeCapacities(t);

  int m; std::cin >> m;
  for (int i = 0; i < m; i++) {
    Point start; std::cin >> start;
    long d; std::cin >> d;
    Face_handle f = t.locate(start);
    Point nearestPoint = t.nearest_vertex(start)->point();
    if (CGAL::squared_distance(start, nearestPoint) < d) {
      std::cout << "n";
      continue;
    }

    if (f->info() >= 4*d) {
      std::cout << "y";
    } else {
      std::cout << "n";
    }
  }
  std::cout << std::endl;


}

int main() {
  std::ios_base::sync_with_stdio(false);

  int n; std::cin >> n;

  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }

  return 0;
}
