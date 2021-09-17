#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;

void testcase() {
  //Read input
  int n; std::cin >> n;
  VI T(n);
  
  for(int i = 0; i < n; i++) {
    std::cin >> T[i];
  }
  
  //Algorithm
  for (int i = 0; i <= (n-3)/2; i++) {
    T[2*i+1] = std::min(T[2*i+1], T[i] - 1);
    T[2*i+2] = std::min(T[2*i+2], T[i] - 1);
  }
  
  std::sort(T.begin(), T.end());
  
  bool solution = true;
  for (int currentTime = 0; currentTime < T.size(); currentTime++) {
    if (T[currentTime] < currentTime + 1) {
      solution = false;
      break;
    }
  }
  
  //Print solution!
  if (solution)
  std::cout << "yes" << std::endl;
  else
  std::cout << "no"  << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
