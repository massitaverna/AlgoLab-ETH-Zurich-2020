#include <iostream>
#include <vector>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;

void testcase() {
  int n; std::cin >> n;
  MI A(n, VI(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      std::cin >> A[i][j];
    }
  }
  
  MI Z(n, VI(n,0));
  for (int i = 0; i < n; i++) {
    Z[i][0] = A[i][0];
    for (int j = 1; j < n; j++) {
      Z[i][j] = Z[i][j-1] + A[i][j];
    }
  }
  
  int total = 0;
  
  for (int j1 = 0; j1 < n; j1++) {
    for (int j2 = j1; j2 < n; j2++) {
      VI S(n, 0);
      int e = 0, o = 0;
      if (j1 != 0) S[0] = Z[0][j2] - Z[0][j1-1];
      else S[0] = Z[0][j2];
      if (S[0]%2 == 0) e++;
      else o++;
      for (int i = 1; i < n; i++) {
        if (j1 != 0) S[i] = S[i-1] + Z[i][j2] - Z[i][j1-1];
        else S[i] = S[i-1] + Z[i][j2];
        if (S[i]%2 == 0) e++;
        else o++;
      }
      total += e*(e-1)/2 + o*(o-1)/2 + e;
    }
  }
  
  std::cout << total << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  
  while (t--) testcase();
}
