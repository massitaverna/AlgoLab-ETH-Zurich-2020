#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

struct Rider {
  K::FT y0;
  K::FT x1;
  K::FT y1;
  int index;
  Rider(double a, double b, double c, int d) : y0(a), x1(b), y1(c), index(d) {}
};

K::FT absValue(const K::FT& x) {
  if (x >= 0) return x;
  return -x;
}
bool yCompare(const Rider& r1, const Rider& r2) {
  return r1.y0 < r2.y0;
}

bool indexCompare(const Rider& r1, const Rider& r2) {
  return r1.index < r2.index;
}

bool greater(const Rider& r1, const Rider& r2) {
  if ((r1.x1 > 0 and r2.x1 > 0) or (r1.x1 < 0 and r2.x1 < 0)) {
    return (r1.y1 - r1.y0)*r2.x1 > (r2.y1 - r2.y0)*r1.x1;
  }
  return (r1.y1 - r1.y0)*r2.x1 < (r2.y1 - r2.y0)*r1.x1;
}

bool absGreater(const Rider& r1, const Rider& r2) {
  return absValue((r1.y1 - r1.y0)*r2.x1) > absValue((r2.y1 - r2.y0)*r1.x1);
}

void testcase() {
  // Read input
  int n; std::cin >> n;
  std::vector<Rider> riders;
  riders.reserve(n);
  for (int i = 0; i < n; i++) {
    double a, b, c; std::cin >> a >> b >> c;
    riders.push_back(Rider(a, b, c, i));
  }
  
  //Algorithm
  std::sort(riders.begin(), riders.end(), yCompare);
  
  std::vector<Rider> S;
  S.reserve(n);
  
  for (Rider& curr : riders) {
    for (int i = S.size() - 1; i >= 0; i--) {
      const Rider& rs = S[i];
      if (curr.y1 < curr.y0) {
        if (absGreater(rs, curr) and rs.y0 < rs.y1) {
          S.pop_back();
        } else break;
      }
      else {
        if (greater(rs, curr)) {
          S.pop_back();
        } else break;
      }
    }
    
    if (curr.y1 < curr.y0) {
      if (S.empty() or !greater(S.back(), curr)) {
         S.push_back(curr);
      }
    }
    else {
       S.push_back(curr);
    }
  }
  
  std::sort(S.begin(), S.end(), indexCompare);
  
  //Print solution!
  for (const Rider& r : S) {
    std::cout << r.index << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
