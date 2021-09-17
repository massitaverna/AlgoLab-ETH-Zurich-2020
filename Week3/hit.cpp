#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;


double floor_to_double(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a--;
  while (a + 1 <= x) a++;
  return a;
}

double ceil_to_double(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a++;
  while (a - 1 >= x) a--;
  return a;
}


int main() {
  std::ios_base::sync_with_stdio(false);

  int n; std::cin >> n;
  long px, py, qx, qy;
  while (n != 0) {
    //Read input
    std::cin >> px;
    std::cin >> py;
    std::cin >> qx;
    std::cin >> qy;
    R r(P(px,py), P(qx,qy));
    std::vector<S> s;

    for (int i = 0; i < n; i++) {
      std::cin >> px;
      std::cin >> py;
      std::cin >> qx;
      std::cin >> qy;
      
      s.push_back(S(P(px,py),P(qx,qy)));
    }

    //Algorithm
    bool res = false;
    for (const S& seg : s) {
      if (CGAL::do_intersect(r, seg)) {
        res = true;
        break;
      }
    }
    
    //Print solution!
    std::cout << (res ? "yes" : "no") << std::endl;

    //Read next n
    std::cin >> n;

  };


  return 0;
}
