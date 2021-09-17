#include <iostream>
#include <vector>
#include <cmath>
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

  int n, d; std::cin >> n;

  while (n != 0) {
    std::cin >> d;

    //Read input
    MI A(n, VI(d));
    VI b(n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < d; j++) {
        std::cin >> A[i][j];
      }
      std::cin >> b[i];
    }

    //Algorithm
    Program lp = Program(CGAL::SMALLER, false, 0, false, 0);

    for (int i = 0; i < n; i++) {
      int norm = 0;
      for (int j = 0; j < d; j++) {
        lp.set_a(j, i, A[i][j]);
        lp.set_a(j, i+n, A[i][j]);
        norm += A[i][j]*A[i][j];
      }
      norm = std::sqrt(norm);
      lp.set_b(i, b[i]);
      lp.set_a(d, i+n, norm);
      lp.set_b(i+n, b[i]);
    }

    lp.set_l(d, 0);
    lp.set_c(d, -1);

    Solution s = CGAL::solve_linear_program(lp, ET());

    //Print solution!
    if (s.is_infeasible()) {
      std::cout << "none" << std::endl;
    } else if (s.is_unbounded()) {
      std::cout << "inf" << std::endl;
    }
    else {
      double radius = std::floor(std::abs(s.objective_value().numerator().to_double())
            /
            std::abs(s.objective_value().denominator().to_double()));
      std::cout << radius << std::endl;
    }

    std::cin >> n;
  }

  return 0;
}



