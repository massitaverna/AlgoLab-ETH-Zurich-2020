#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef std::vector<Point> VP;
typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


double ceil_to_double(const CGAL::Quotient<ET>& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a++;
  while (a-1 >= x) a--;
  return a;
}


void testcase() {
  // Read input
  int n, m;
  IT s;
  std::cin >> n >> m >> s;
  VP nobles(n);
  VP commons(m);
  
  for (int i = 0; i < n; i++) {
    std::cin >> nobles[i];
  }
  for (int i = 0; i < m; i++) {
    std::cin >> commons[i];
  }
  
  //Algorithm
  const int a   = 0;
  const int b   = 1;
  const int c   = 2;
  const int k   = 3;
  const int min = 4;
  int constr = 0;

  Program lp(CGAL::SMALLER, false, 0, false, 0);
  // a == 1
  lp.set_l(a, true, 1);
  lp.set_u(a, true, 1);
  
  IT a_coeff = 0;
  IT b_coeff = 0;
  IT c_coeff = 0;

  /* Cersei's constraint */
  for (int i = 0; i < n; ++i) {
    lp.set_a(a, constr, nobles[i].x()); 
    lp.set_a(b, constr, nobles[i].y());
    lp.set_a(c, constr, 1);
    a_coeff += -nobles[i].x();
    b_coeff += -nobles[i].y();
    c_coeff += -1;
    constr++;
  }
  for (int i = 0; i < m; ++i) {
    lp.set_a(a, constr, -commons[i].x()); 
    lp.set_a(b, constr, -commons[i].y());
    lp.set_a(c, constr, -1);
    a_coeff += commons[i].x();
    b_coeff += commons[i].y();
    c_coeff += 1;
    constr++;
  }
  
  /* Jaime's constraint */
  for (int i = 0; i < n; i++) {
    // constraint: -a*min <= a*y0 - b*x0 - k <= a*min
    // 1) a*y0 - b*x0 -k -a*min <= 0
    lp.set_a(a, constr, nobles[i].y());
    lp.set_a(b, constr, -nobles[i].x());
    lp.set_a(k, constr, -1);
    lp.set_a(min, constr, -1);
    constr++;
    // 2) b*x0 - a*y0 + k -a*min <= 0
    lp.set_a(a, constr, -nobles[i].y());
    lp.set_a(b, constr, nobles[i].x());
    lp.set_a(k, constr, 1);
    lp.set_a(min, constr, -1);
    constr++;
  }
  for (int i = 0; i < m; i++) {
    lp.set_a(a, constr, commons[i].y());
    lp.set_a(b, constr, -commons[i].x());
    lp.set_a(k, constr, -1);
    lp.set_a(min, constr, -1);
    constr++;
    lp.set_a(a, constr, -commons[i].y());
    lp.set_a(b, constr, commons[i].x());
    lp.set_a(k, constr, 1);
    lp.set_a(min, constr, -1);
    constr++;
  }
  
  lp.set_c(min, 1);
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  if (sol.is_infeasible()) {
    std::cout << "Yuck!" << std::endl;
    return;
  }
  
  /* Tywin's constraint */
  if (s >= 0) {
    lp.set_a(a, constr, a_coeff);
    lp.set_a(b, constr, b_coeff);
    lp.set_a(c, constr, c_coeff);
    lp.set_b(constr, s);
    constr++;
    
    sol = CGAL::solve_linear_program(lp, ET());
    if (sol.is_infeasible()) {
      std::cout << "Bankrupt!" << std::endl;
      return;
    }
  }
  
  double min_water_pipe = ceil_to_double(sol.objective_value());
  
  //Print solution!
  std::cout << min_water_pipe << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cout << std::fixed << std::setprecision(0);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
