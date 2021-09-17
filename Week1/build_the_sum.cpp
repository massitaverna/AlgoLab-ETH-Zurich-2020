#include <iostream>

void testcase() {
  int n; std::cin >> n;
  int sum = 0;
  
  for (int i = 0; i < n; i++) {
    int val; std::cin >> val;
    sum += val;
  }

  std::cout << sum << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
  
  return 0;
}
