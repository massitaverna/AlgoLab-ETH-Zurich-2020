#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::vector<int> VI;
typedef std::vector<VI> MI;


int main() {
  std::ios_base::sync_with_stdio(false);

  int n, m; std::cin >> n >> m;

  while (n != 0 or m != 0) {

    //Read input
    VI min(n), max(n);
    for (int i = 0; i < n; i++) {
      std::cin >> min[i] >> max[i];
    }

    VI price(m);
    MI C(m, VI(n));
    for (int i = 0; i < m; i++) {
      std::cin >> price[i];
      for (int j = 0; j < n; j++) {
        std::cin >> C[i][j];
      }
    }

    //Algorithm
    Program lp = Program(CGAL::SMALLER, true, 0, false, 0);

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        lp.set_a(j, i, C[j][i]);
        lp.set_a(j, i+n, -C[j][i]);
      }
      lp.set_b(i, max[i]);
      lp.set_b(i+n, -min[i]);
    }

    for (int j = 0; j < m; j++) {
      lp.set_c(j, price[j]);
    }

    Solution s = CGAL::solve_linear_program(lp, ET());

    //Print solution!
    if (s.is_infeasible()) {
      std::cout << "No such diet." << std::endl;
    } else {
      double cost = std::floor(s.objective_value().numerator().to_double()
            /
            s.objective_value().denominator().to_double());
      std::cout << cost << std::endl;
    }

    std::cin >> n >> m;
  }

  return 0;
}
