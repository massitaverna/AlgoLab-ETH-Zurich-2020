#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


int roundDown(int a, int b) {
  if (a%b == 0) return a/b;
  else if (a < 0) return a/b - 1;
  else return a/b;
}

int roundUp(int a, int b) {
  if (a%b == 0) return a/b;
  else if (a > 0) return a/b + 1;
  else return a/b;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  Program lp1 = Program(CGAL::SMALLER, true, 0, false, 0);
  Program lp2 = Program(CGAL::SMALLER, false, 0, true, 0);
  Program* current;

  const int X = 0;
  const int Y = 1;
  const int Z = 2;

  lp1.set_a(X, 0, 1);  lp1.set_a(Y, 0, 1); lp1.set_b(0, 4);
  lp1.set_a(X, 1, -1); lp1.set_a(Y, 1, 1); lp1.set_b(1, 1);
  lp1.set_a(X, 2, 4);  lp1.set_a(Y, 2, 2);

  lp2.set_a(X, 0, -1); lp2.set_a(Y, 0, -1); lp2.set_b(0, 4);
  lp2.set_a(X, 1, 1);  lp2.set_a(Y, 1, -1); lp2.set_b(1, 1);
  lp2.set_a(X, 2, -4); lp2.set_a(Y, 2, -2); lp2.set_a(Z, 2, -1);
  lp2.set_c(Z, 1);

  int p, a, b;
  std::cin >> p;

  while (p != 0) {
    std::cin >> a >> b;
    if (p == 1) {
      lp1.set_b(2, a*b);
      lp1.set_c(X, a); lp1.set_c(Y, -b);
      current = &lp1;
    } else {
      lp2.set_b(2, a*b);
      lp2.set_c(X, a); lp2.set_c(Y, b);
      current = &lp2;
    }

    Solution s = CGAL::solve_linear_program(*current, ET());
    if (s.is_infeasible()) {
      std::cout << "no" << std::endl;
    }
    else if (s.is_unbounded()) {
      std::cout << "unbounded" << std::endl;
    }
    else {
      CGAL::Quotient<ET> res = *(s.variable_values_begin());
      int solution;
      if (p == 1) {
        solution = roundDown((int) -s.objective_value().numerator().to_double(),
                 (int) s.objective_value().denominator().to_double());
      } else {
        solution = roundUp((int) s.objective_value().numerator().to_double(),
               (int) s.objective_value().denominator().to_double());
      }

      std::cout << solution << std::endl;
    }

    std::cin >> p;
  }

  return 0;
}



