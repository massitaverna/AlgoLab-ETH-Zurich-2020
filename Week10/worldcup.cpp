#include <iostream>
#include <vector>
#include <set>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef std::vector<Point> VP;
typedef std::vector<int> VI;
typedef std::vector<VI> MI;

struct CLine {
  double x;
  double y;
  double sqradius;
  CLine(double a, double b, double c) : x(a), y(b), sqradius(c) {}
};

bool operator < (const CLine& c1, const CLine& c2) {
  if (c1.x != c2.x) return c1.x < c2.x;
  else if (c1.y != c2.y) return c1.y < c2.y;
  return c1.sqradius < c2.sqradius;
}

double floor_to_double(CGAL::Quotient<ET> x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

double ceil_to_double(CGAL::Quotient<ET> x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase() {
  int n, m, c; std::cin >> n >> m >> c;
  VP WLocations(n);
  VI WSupplies(n);
  VI WAlcohol(n);
  VP SLocations(m);
  VI SDemands(m);
  VI SUpper(m);
  std::vector<std::vector<IT>> revenues(n, std::vector<IT>(m, 0));
  std::vector<CLine> allLines;
  allLines.reserve(c);
  
  for (int i = 0; i < n; i++) {
    int x, y, s, a;
    std::cin >> x >> y >> s >> a;
    WLocations[i] = Point(x, y);
    WSupplies[i] = s;
    WAlcohol[i] = a;
  }
  for (int i = 0; i < m; i++) {
    int x, y, d, u;
    std::cin >> x >> y >> d >> u;
    SLocations[i] = Point(x, y);
    SDemands[i] = d;
    SUpper[i] = u;
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int rev; std::cin >> rev;
      revenues[i][j] = 100*rev;
    }
  }
  for (int i = 0; i < c; i++) {
    int x, y;
    double r;
    std::cin >> x >> y >> r;
    allLines.push_back(CLine(x, y, r*r));
  }
  
  std::vector<std::set<CLine>> WLines(n);
  for (int w = 0; w < n; w++) {
    for (const CLine& cl : allLines) {
      if ((WLocations[w].x()-cl.x)*(WLocations[w].x()-cl.x) +
          (WLocations[w].y()-cl.y)*(WLocations[w].y()-cl.y) <= cl.sqradius) {
        WLines[w].insert(cl);
      }
    }
  }
  std::vector<std::set<CLine>> SLines(m);
  for (int s = 0; s < m; s++) {
    for (const CLine& cl : allLines) {
      if ((SLocations[s].x()-cl.x)*(SLocations[s].x()-cl.x) +
          (SLocations[s].y()-cl.y)*(SLocations[s].y()-cl.y) <= cl.sqradius) {
        SLines[s].insert(cl);
      }
    }
  }
  
  MI crossings(n, VI(m, 0));
  for (int w = 0; w < n; w++) {
    for (int s = 0; s < m; s++) {
      for (const CLine& cl : WLines[w]) {
        if (SLines[s].find(cl) == SLines[s].end()) {
          crossings[w][s]++;
        }
      }
      for (const CLine& cl : SLines[s]) {
        if (WLines[w].find(cl) == WLines[w].end()) {
          crossings[w][s]++;
        }
      }
    }
  }
  for (int w = 0; w < n; w++) {
    for (int s = 0; s < m; s++) {
      revenues[w][s] -= crossings[w][s];
    }
  }
  
  Program lp(CGAL::SMALLER, false, 0, false, 0);
  int costr = 0;
  for (int s = 0; s < m; s++) {
    for (int w = 0; w < n; w++) {
      lp.set_a(w*20 + s, costr, 1);
    }
    lp.set_b(costr, SDemands[s]);
    costr++;
  }
  for (int s = 0; s < m; s++) {
    for (int w = 0; w < n; w++) {
      lp.set_a(w*20 + s, costr, -1);
    }
    lp.set_b(costr, -SDemands[s]);
    costr++;
  }
  for (int w = 0; w < n; w++) {
    for (int s = 0; s < m; s++) {
      lp.set_a(w*20 + s, costr, 1);
    }
    lp.set_b(costr, WSupplies[w]);
    costr++;
  }
  for (int s = 0; s < m; s++) {
    for (int w = 0; w < n; w++) {
      lp.set_a(w*20 + s, costr, WAlcohol[w]);
    }
    lp.set_b(costr, 100*SUpper[s]);
    costr++;
  }
  for (int s = 0; s < m; s++) {
    for (int w = 0; w < n; w++) {
      lp.set_c(w*20+s, -revenues[w][s]);
    }
  }
  
  Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
  if (s.is_infeasible()) {
    std::cout << "RIOT!" << std::endl;
  }
  else {
    auto revenue = -s.objective_value();
    revenue = revenue/100;
    double total;
    if (revenue < 0) {
      total = -ceil_to_double(-revenue);
    } else {
      total = floor_to_double(revenue);
    }
    std::cout << total << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);
  int t; std::cin >> t;
  while (t--) testcase();
}

