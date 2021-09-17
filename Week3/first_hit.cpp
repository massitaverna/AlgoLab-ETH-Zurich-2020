/*
Two "optimisations" need to be done in order to get 100 points:
  1. Clip the initial ray
     When we find intersection with point P and ray starts in A,
     then we substitute the ray with the segment AP.
     By doing so, next calls to do_intersect() will return true iff
     we find a closer intersection point. This allows us to construct
     the squared_distance() only for the new possible solutions
     (as a consequence, a comparison is no mor required)
  2. Random shuffle
     Even with the above strategy, the input may be in an adversarial way,
     such that intersection points are in decreasing order by distance.
     To avoid this worst-case scenario that would make our clipping strategy
     useless, we simply shuffle the set of segments randomly
*/

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;


double floor_to_double(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a--;
  while (a + 1 <= x) a++;
  return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  std::cout << std::fixed;
  std::cout << std::setprecision(0);

  int n; std::cin >> n;
  long px, py, qx, qy;
  while (n != 0) {
    //Read input
    std::cin >> px;
    std::cin >> py;
    std::cin >> qx;
    std::cin >> qy;
    P start(px,py);
    R r(start, P(qx,qy));
    std::vector<S> s;

    for (int i = 0; i < n; i++) {
      std::cin >> px;
      std::cin >> py;
      std::cin >> qx;
      std::cin >> qy;
      
      s.push_back(S(P(px,py),P(qx,qy)));
    }
    std::random_shuffle(s.begin(), s.end());

    //Algorithm
    P res;
    K::FT distance;
    bool at_least_one = false;
    S raySegment;

    auto it = s.cbegin();
    for (; it != s.cend(); it++) {
      S seg = *it;
      if (CGAL::do_intersect(r, seg)) {
        at_least_one = true;
        auto o = CGAL::intersection(r, seg);
        if (const S* os = boost::get<S>(&*o)) { // If segment is on the ray
          distance = CGAL::squared_distance(*os, start);
          if (CGAL::squared_distance((*os).source(), start) == distance)
            res = (*os).source();
          else
            res = (*os).target();
          raySegment = S(start, res);
          break;
        } else { //Intersection is a point
          const P* op = boost::get<P>(&*o);
          distance = CGAL::squared_distance(*op, start);
          res = *op;
          raySegment = S(start, res);
          break;
        }
      }
    }

    if (it != s.cend()) it++;
    for (; it != s.cend(); it++) {
      S seg = *it;

      if (CGAL::do_intersect(raySegment, seg)) {
        auto o = CGAL::intersection(raySegment, seg);
        K::FT d;
        if (const S* os = boost::get<S>(&*o)) { // If segment is on the ray
          d = CGAL::squared_distance(*os, start);
          if (d < distance) {
            distance = d;;
            if (CGAL::squared_distance((*os).source(), start) == distance) {
              res = (*os).source();
            }
            else {
              res = (*os).target();
            }
            raySegment = S(start, res);
          }
        } else { //Intersection is a point
          const P* op = boost::get<P>(&*o);
          d = CGAL::squared_distance(*op, start);
          if (d < distance) {
            distance = d;
            res = *op;
            raySegment = S(start, res);
          }  
        }
      }
    }
    
    //Print solution!
    if (at_least_one) {
      std::cout << floor_to_double(res.x()) << " " << floor_to_double(res.y()) << std::endl;
    } else {
      std::cout << "no" << std::endl;
    }

    //Read next n
    std::cin >> n;

  };


  return 0;
}
