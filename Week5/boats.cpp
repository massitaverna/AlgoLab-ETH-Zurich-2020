#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>


struct Boat {
  int l;
  int p;

  Boat(int a, int b) : l(a), p(b) {}
};


bool ringCompare (const Boat& b1, const Boat& b2) {
  return b1.p < b2.p;
}


void testcase() {
  //Read input
  int n; std::cin >> n;
  std::vector<Boat> boats;
  boats.reserve(n);
  for (int i = 0; i < n; i++) {
    int l, p; std::cin >> l >> p;
    boats.push_back(Boat(l, p));
  }

  //Algorithm -- O(n)
  std::sort(boats.begin(), boats.end(), ringCompare);

  int last_f = boats[0].p;
  int res = 1;

  int f_min = std::numeric_limits<int>::max();
  int idx_min;
  for (int i = 1; i < n; i++) {
    if (last_f <= boats[i].p and boats[i].p <= f_min) {
      int f_pot;
      if (last_f + boats[i].l <= boats[i].p) {
        f_pot = boats[i].p;
      } else {
        f_pot = last_f + boats[i].l;
      }
      if (f_pot < f_min) {
        f_min = f_pot;
        idx_min = i;
      }
      if (i == n-1 or boats[i+1].p > f_min) {
        last_f = f_min;
        res++;
        i = idx_min;
        f_min = std::numeric_limits<int>::max();
      }
    }

  }

  //Print solution!
  std::cout << res << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }

  return 0;
}
