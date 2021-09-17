#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;

void testcase() {
  int n, k, w; std::cin >> n >> k >> w;
  VI costs(n);
  MI waterways(w);
  for (int i = 0; i < n; i++) {
    std::cin >> costs[i];
  }
  for (int i = 0; i < w; i++) {
    int l; std::cin >> l;
    for (int j = 0; j < l; j++) {
      int island; std::cin >> island;
      waterways[i].push_back(costs[island]);
    }
  }
  
  int max_cnt = 0;
  for (int i = 0; i < w; i++) {
    auto s = waterways[i].begin();
    auto e = waterways[i].begin();
    int sum = *s;
    int cnt = 1;
    auto end = waterways[i].end();
    while (s != end and e != end) {
      if (sum == k and cnt > max_cnt) {
        max_cnt = cnt;
      }
      if (sum >= k) {
        sum -= *s;
        s++;
        cnt--;
        if (cnt == 0 and s != end) {
          e = s;
          sum = *s;
          cnt = 1;
        }
      }
      else {
        e++;
        sum += *e;
        cnt++;
      }
    }
  }
  
  // num_ironmen --> {num_waterway, length}
  std::vector<std::pair<int, int>> bestChoices(k+1, {-1, -1});
  for (int i = 0; i < w; i++) {
    int sum = 0;
    for (int j = 0; j < (int) waterways[i].size(); j++) {
      sum += waterways[i][j];
      if (sum >= k) break;
      if (bestChoices[sum].second < j+1) {
        bestChoices[sum] = {i, j+1};
      }
    }
  }
  for (int i = 0; i < w; i++) {
    int sum = 0;
    for (int j = 1; j < (int) waterways[i].size(); j++) {
      sum += waterways[i][j];
      if (sum >= k) break;
      int neededMen = k - sum;
      auto bc = bestChoices[neededMen];
      if (bc.first != -1 and bc.first != i) {
        max_cnt = std::max(max_cnt, bc.second + j);
      }
    }
  }
  
  
  std::cout << max_cnt << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
