#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;


bool topple_next(const VI& A, int c) {
  return A[c] > 1;
}

int furthest_toppled(const VI& A, int c) {
  return c + A[c] - 1;
}

void testcase() {

  //Read input
  int n; std::cin >> n;
  VI A;

  
  for (int i = 0; i < n; i++) {
    int val; std::cin >> val;
    A.push_back(val);
  }

  //Algorithm
  int current = 0, next = 0;
  
  while (true) {
    int last_fallen = furthest_toppled(A, current);
    next = last_fallen + 1;

    if (next >= n) break; //All tiles fell down

    //Trying to make 'next' fall down (or a tile after 'next')
    int best_one;
    while (last_fallen > current)  {
      if (furthest_toppled(A, last_fallen) >= next) {
        best_one = last_fallen;
        next = furthest_toppled(A, last_fallen) + 1;
      }
      last_fallen--;
    }

    //If I succeeded in making a tile fall ('next' or after it)
    if (next > furthest_toppled(A, current) + 1) {
      current = best_one;
    } else {
      break; // If I can't make any other tail after previous 'next' fall, then I am finished
    }
  }

  //Print solution!
  std::cout << std::min(next, n) << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
  
  return 0;
}
