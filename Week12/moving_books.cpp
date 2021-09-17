#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::multiset<int, std::greater<int>> Multiset;


void testcase() {
  // Read input
  int n, m; std::cin >> n >> m;
  VI friends(n);
  Multiset books;

  for (int i = 0; i < n; i++) {
    std::cin >> friends[i];
  }
  for (int i = 0; i < m; i++) {
    int b; std::cin >> b;
    books.insert(b);
  }
  
  // Algorithm
  std::sort(friends.begin(), friends.end(), std::greater<int>());
  
  if (friends[0] < *(books.begin())) {
    std::cout << "impossible" << std::endl;
    return;
  }
  
  int rounds = 0;
  while(!books.empty()) {
    for (int i = 0; i < n; i++) {
      auto it = books.lower_bound(friends[i]);
      if (it == books.end()) break;
      books.erase(it);
    }
    rounds++;
  }
  
  std::cout << 3*rounds - 1 << std::endl;

}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
