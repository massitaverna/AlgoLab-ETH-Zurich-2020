#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef K::Point_2 Point;
typedef std::vector<Point> VP;

double ceil_to_double(const EK::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

EK::FT S_to_hrs(const EK::FT& S) {
  return CGAL::sqrt((CGAL::sqrt(S)-1)/2);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);

  int n; std::cin >> n;

  while (n != 0) {

    //Read input
    int l, b, r, t; std::cin >> l >> b >> r >> t;

    VP points;
    points.reserve(n);
    for (int i = 0; i < n; i++) {
      int x, y; std::cin >> x >> y;
      points.push_back(Point(x, y));
    }

    //Algorithm
    Triangulation tr;
    tr.insert(points.begin(), points.end());

    std::vector<K::FT> deaths;
    deaths.reserve(n);

    for (auto vh = tr.finite_vertices_begin(); vh != tr.finite_vertices_end(); vh++) {
      K::FT min = 4*std::pow(vh->point().x() - l, 2);

      min = std::min(min, 4*std::pow(vh->point().x() - l, 2));
      min = std::min(min, 4*std::pow(vh->point().x() - r, 2));
      min = std::min(min, 4*std::pow(vh->point().y() - t, 2));
      min = std::min(min, 4*std::pow(vh->point().y() - b, 2));

      if (n > 1) {
        Triangulation::Edge_circulator c = tr.incident_edges(vh);
        do {
          if (!tr.is_infinite(c)) {
            min = std::min(min, tr.segment(c).squared_length());
          }
        } while (++c != tr.incident_edges(vh));
      }
      
      deaths.push_back(min);
    }

    std::sort(deaths.begin(), deaths.end());
    EK::FT first = S_to_hrs(EK::FT(deaths[0]));
    EK::FT median = S_to_hrs(EK::FT(deaths[n/2]));
    EK::FT last = S_to_hrs(EK::FT(deaths[n-1]));

    //Print solution!
    std::cout << std::fixed << std::setprecision(0)
        << ceil_to_double(first) << " "
        << ceil_to_double(median) << " "
        << ceil_to_double(last)
        << std::endl;  


    std::cin >> n;
  }

  return 0;
}
