#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

enum Color {White = 0, Grey = 1, Black = 2};
struct VInfo {
  Color color = White;
  int frequency = -1;
  int connectedComponent;
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<VInfo, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef K::Point_2 Point;
typedef std::vector<Point> VP;
typedef std::pair<Point, Point> Clue;
typedef std::vector<Clue> VC;
typedef std::queue<Triangulation::Vertex_handle> VertexQueue;

int opposite(int x) {
  return (x == 0) ? 1 : 0;
}

bool checkNetwork(Triangulation& t, double sqRadius) {
  VertexQueue Q;
  int connectedComponent = -1;
  std::vector<VP> pts(2, VP());
  for (auto ext_vh = t.finite_vertices_begin(); ext_vh != t.finite_vertices_end(); ext_vh++) {
    if (ext_vh->info().color == White) {
      connectedComponent++;
      ext_vh->info().frequency = 0;
      pts[0].push_back(ext_vh->point());
      ext_vh->info().color = Grey;
      ext_vh->info().connectedComponent = connectedComponent;
      Q.push(ext_vh);
    }
    while (!Q.empty()) {
      auto vh = Q.front();
      Q.pop();
      vh->info().color = Black;
      Triangulation::Vertex_circulator c = t.incident_vertices(vh);
      Point p = vh->point();
      do {
        if (!t.is_infinite(c)) {
          Point q = c->point();
          double sqDist = CGAL::squared_distance(p, q);
          if (sqDist <= sqRadius) {
            if (c->info().frequency == vh->info().frequency) {
              return false;
            }
            if (c->info().color == White) {
              c->info().frequency = opposite(vh->info().frequency);
              c->info().connectedComponent = connectedComponent;
              pts[c->info().frequency].push_back(c->point());
              c->info().color = Grey;
              Q.push(c->handle());
            }
          }
        }
      } while (++c != t.incident_vertices(vh));
    }  
  }

  Triangulation t1;
  t1.insert(pts[0].begin(), pts[0].end());
  for (auto e = t1.finite_edges_begin(); e != t1.finite_edges_end(); e++) {
    if (t1.segment(e).squared_length() <= sqRadius) {return false;}
  }
  Triangulation t2;
  t2.insert(pts[1].begin(), pts[1].end());
  for (auto e = t2.finite_edges_begin(); e != t2.finite_edges_end(); e++) {
    if (t2.segment(e).squared_length() <= sqRadius) {return false;}
  }

  return true;
}

void testcase() {
  //Read input
  int n, m; std::cin >> n >> m;
  double r; std::cin >> r;
  VP stations;
  stations.reserve(n);
  VC clues;
  clues.reserve(m);

  for (int i = 0; i < n; i++) {
    int x, y; std::cin >> x >> y;
    stations.push_back(Point(x, y));
  }

  for (int i = 0; i < m; i++) {
    int x, y; std::cin >> x >> y;
    Point a = Point(x, y);
    std::cin >> x >> y;
    Point b = Point(x, y);
    clues.push_back(std::make_pair(a, b));
  }

  //Algorithm
  Triangulation t;
  t.insert(stations.begin(), stations.end());

  bool networkOk = checkNetwork(t, r*r);
  if (!networkOk) {
    for (int i = 0; i < m; i++) {
      std::cout << "n";
    }
    std::cout << std::endl;
    return;
  }
  for (int i = 0; i < m; i++) {
    Clue clue = clues[i];
    Point a = clue.first;
    Point b = clue.second;
    if (CGAL::squared_distance(a, b) <= r*r) {
      std::cout << "y";
    } else {
      auto begin = t.nearest_vertex(a);
      auto end = t.nearest_vertex(b);
      if (CGAL::squared_distance(a, begin->point()) <= r*r and
          CGAL::squared_distance(b, end->point()) <= r*r and
          begin->info().connectedComponent == end->info().connectedComponent
      ) {
        std::cout << "y";
      } else {
        std::cout << "n";
      }
    }
  }
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);;

  int n; std::cin >> n;

  while (n--) {
    testcase();
  }

  return 0;
}
