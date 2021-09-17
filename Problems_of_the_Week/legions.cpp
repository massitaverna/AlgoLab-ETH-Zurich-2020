#include <iostream>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(CGAL::Quotient<CGAL::Gmpz> x) {
  double a = CGAL::to_double(x);
  a = std::floor(a);
  return a;
}
void testcase() {
  int xs, ys, n; std::cin >> xs >> ys >> n;
  const int X = 0;
  const int Y = 1;
  const int T = 2;
  Program lp(CGAL::SMALLER, false, 0, false, 0);
  
  for (int i = 0; i < n; i++) {
    IT a, b, c, v; std::cin >> a >> b >> c >> v;
    IT norm = a*a + b*b;
    norm = std::sqrt(norm);
    if (a*xs + b*ys + c > 0) {
      a = -a;
      b = -b;
      c = -c;
    }
    lp.set_a(X, i, a);
    lp.set_a(Y, i, b);
    lp.set_a(T, i, v*norm);
    lp.set_b(i, -c);
  }
  lp.set_l(T, true, 0);
  lp.set_c(T, -1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  double preparationTime = floor_to_double(-s.objective_value());
  
  std::cout << preparationTime << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}
