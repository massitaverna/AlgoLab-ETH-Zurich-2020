#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;
typedef std::vector<bool> VB;
typedef std::pair<int, int> MemoElement;
typedef std::vector<MemoElement> Memo;

int N, K;
Memo* _memo;
#define memo(l, d, neverUsed) (*_memo)[neverUsed*(K+1)*(N+1) + l*(N+1) + d]


MemoElement f(int k, int n, bool neverUsed, const VI& costs, const VI& litres) {
  if (k <= 0) return {0, 0};
  if (memo(k, n, neverUsed).first != -1) return memo(k, n, neverUsed);

  MemoElement res = f(k-litres[n-1], n, false, costs, litres);
  res.first += costs[n-1];
  res.second += neverUsed;
  MemoElement res2;
  res2.first = std::numeric_limits<int>::max();
  if (n > 1) {
    res2 = f(k, n-1, true, costs, litres);
  }
  
  if (res.first < res2.first) {
    memo(k, n, neverUsed) = res;
  } else if (res.first > res2.first) {
    memo(k, n, neverUsed) = res2;
  } else if (res.second > res2.second) {
    memo(k, n, neverUsed) = res;
  } else {
    memo(k, n, neverUsed) = res2;
  }

  return memo(k, n, neverUsed);
}

void testcase() {
  std::cin >> N >> K;
  VI costs(N);
  VI litres(N);
  for (int i = 0; i < N; i++) {
    std::cin >> costs[i] >> litres[i];
  }
  
  _memo = new Memo(2*(K+1)*(N+1), {-1, 0});
  MemoElement solution = f(K, N, true, costs, litres);
  delete _memo;
  
  int cost = solution.first;
  int diversity = solution.second;
  
  std::cout << cost << " " << diversity << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
