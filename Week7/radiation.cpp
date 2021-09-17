#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpzf IT;
typedef CGAL::Gmpzf ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::vector<int> VI;
typedef std::vector<VI> MI;


bool existsPolynomial(int d, const MI& posPoints, const MI& negPoints, VI& solutions) {
  const int X = 0, Y = 1, Z = 2;
  int var = 0;
  int idx = 0;
  Program lp = Program(CGAL::SMALLER, false, 0, false, 0);
  CGAL::Quadratic_program_options options;
  options.set_pricing_strategy(CGAL::QP_BLAND);


  if (solutions[d] != -1) {
    return solutions[d];
  }

  for (const VI& p : posPoints) {
    IT x = 1;
    for (int i = 0; i <= d; i++) {
      IT y = 1;
      for (int j = 0; j <= d-i; j++) {
        IT z = 1;
        for (int k = 0; k <= d-i-j; k++) {
          lp.set_a(var, idx, -x*y*z);
          z = z*p[Z];
          var++;
        }
        y = y*p[Y];
      }
      x = x*p[X];
    }
    lp.set_b(idx, -1);
    var = 0;
    idx++;
  }

  for (const VI& p : negPoints) {
    IT x = 1;
    for (int i = 0; i <= d; i++) {
      IT y = 1;
      for (int j = 0; j <= d-i; j++) {
        IT z = 1;
        for (int k = 0; k <= d-i-j; k++) {
          lp.set_a(var, idx, x*y*z);
          z = z*p[Z];
          var++;
        }
        y = y*p[Y];
      }
      x = x*p[X];
    }
    lp.set_b(idx, -1);
    var = 0;
    idx++;
  }

  Solution s = CGAL::solve_linear_program(lp, ET(), options);
  if (s.is_infeasible()) {
    solutions[d] = 0;
    return false;
  }
  else {
    solutions[d] = 1;
    return true;
  }
}


void testcase() {
  //Read input
  int h, t; std::cin >> h >> t;
  MI posPoints, negPoints;

  for (int i = 0; i < h; i++) {
    VI temp(3);
    std::cin >> temp[0] >> temp[1] >> temp[2];
    negPoints.push_back(temp);
  }
  for (int i = 0; i < t; i++) {
    VI temp(3);
    std::cin >> temp[0] >> temp[1] >> temp[2];
    posPoints.push_back(temp);
  }

  //Algorithm
  std::vector<int> solutions(31, -1);
  if (existsPolynomial(0, posPoints, negPoints, solutions)) {
    std::cout << 0 << std::endl;
    return;
  }

  // 1. Exponential search
  int d = 1;
  int prev = 0;
  while (d <= 30) {
    if (existsPolynomial(d, posPoints, negPoints, solutions)) {
      break;
    }
    prev = d;
    d = 2*d;
  }

  // 2. Binary search
  int start = prev + 1;
  int end = std::min(d, 30);

  while (start < end) {
    if (existsPolynomial((start+end)/2, posPoints, negPoints, solutions)) {
      end = (start+end)/2;
    } else {
      start = (start+end)/2 + 1;
    }
  }

  //Print solution!
  if (existsPolynomial(start, posPoints, negPoints, solutions)) {
    std::cout << start << std::endl;
  } else {
    std::cout << "Impossible!" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0); std::cout.tie(0);

  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }

  return 0;
}
