#include <iostream>
#include <vector>
#include <utility>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;

struct Triangle {
  std::vector<Segment> sides;
};

bool inside(const Point& p, const Triangle& t) {
  return
  !CGAL::left_turn(t.sides[0].source(), t.sides[0].target(), p) and
  !CGAL::left_turn(t.sides[1].source(), t.sides[1].target(), p) and
  !CGAL::left_turn(t.sides[2].source(), t.sides[2].target(), p);
}


void testcase() {
  int m; std::cin >> m;
  int n; std::cin >> n;

  std::vector<Segment> legs(m-1);
  std::vector<Triangle> triangles(n);

  Point previous;
  int x, y;

  std::cin >> x >> y;
  previous = Point(x, y);
  for (int i = 1; i < m; i++) {
    std::cin >> x >> y;
    Point current = Point(x,y);
    legs[i-1] = Segment(previous, current);
    previous = current;
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 3; j++) {
      std::cin >> x >> y;
      Point p1 = Point(x, y);
      std::cin >> x >> y;
      Point p2 = Point(x, y);
      triangles[i].sides.push_back(Segment(p1, p2));
    }
  }

  //Algorithm
  //Step One. Orientate triangles' sides
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 3; j++) {
      if (!CGAL::right_turn(triangles[i].sides[j].source(), triangles[i].sides[j].target(),
         triangles[i].sides[(j+1)%3].source())) {
        Segment s = Segment(triangles[i].sides[j].target(), triangles[i].sides[j].source());
        triangles[i].sides[j] = s;
      }
    }
  }

  //Step Two. Build matrix: which leg belongs to which triangles
  MI memberships(n, VI(m-1,0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m-1; j++) {
      if (inside(legs[j].source(), triangles[i]) and inside(legs[j].target(), triangles[i])) {
        memberships[i][j] = 1;
      }
    }
  }

  //Step Three. Sliding Window analysis
  int opt_i = 0;
  int opt_j = n+1;
  int i = 0;
  int j = 0;
  VI covered_legs(m-1,0);
  while (j < n) {
    bool complete = true;
    for (int c = 0; c < m-1; c++) {
      covered_legs[c] += memberships[j][c];
    }
    for (int c = 0; c < m-1; c++) {
      if (covered_legs[c] == 0) {
        complete = false;
        break;
      }
    }
    if (complete) {
      if (j-i < opt_j-opt_i) {
        opt_i = i;
        opt_j = j;
      }
      if (i == j) break;
      for (int c = 0; c < m-1; c++) {
        covered_legs[c] -= memberships[i][c];
        covered_legs[c] -= memberships[j][c];
      }
      i++;
    } else {
      j++;
    }
    
  }

  //Print solution!
  std::cout << opt_j - opt_i + 1 << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }

  return 0;
}
