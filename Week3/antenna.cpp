#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 Point;
typedef std::vector<Point> VP;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt KR;
typedef CGAL::Min_circle_2_traits_2<KR> TraitsR;
typedef CGAL::Min_circle_2<TraitsR> Min_circleR;
typedef KR::Point_2 PointR;
typedef std::vector<PointR> VPR;

double ceil_to_double(KR::FT x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a++;
  while (a - 1 >= x) a--;
  
  return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);
  
  int n; std::cin >> n;
  while (n != 0) {
    //Read input
    VP points(n);
    for (int i = 0; i < n; i++) {
      double x, y; std::cin >> x >> y;
      points[i] = Point(x, y);
    }
    
    //Algorithm
    Min_circle mc(points.begin(), points.end(), true);
    Traits::Circle c = mc.circle();
    
    VPR pointsR;
    for (auto it = mc.support_points_begin(); it != mc.support_points_end(); it++) {
      double x = CGAL::to_double(it->x());
      double y = CGAL::to_double(it->y());
      pointsR.push_back(PointR(x, y));
    }
    
    Min_circleR mcr(pointsR.begin(), pointsR.end(), true);
    TraitsR::Circle cr = mcr.circle();
    double sol = ceil_to_double(CGAL::sqrt(cr.squared_radius()));
    
    //Print solution and get next testcase's size
    std::cout << sol << std::endl;
    std::cin >> n;
  }
}
