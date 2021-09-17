#include <iostream>

#include <vector>
#include <algorithm>

#define MAX_VALUE 1024
#define MAX_COINS 1000


typedef std::vector<int> VI;
typedef std::vector<VI> MI;


int FRWL(const VI& X, int i, int j, int m, MI& memo) {
  //Base case
  if (j-i+1 <= m) return std::max(X[i], X[j]);

  //Recall
  if (memo[i][j] != -1) return memo[i][j];

  //Recursive case
  int optionI = MAX_VALUE*MAX_COINS + 1;
  int optionJ = MAX_VALUE*MAX_COINS + 1;
  for (int k = 0; k < m; k++) {
    optionI = std::min(optionI, FRWL(X, i+k+1, j-m+1+k, m, memo));
  }
  for (int k = 0; k < m; k++) {
    optionJ = std::min(optionJ, FRWL(X, i+k, j-m+k, m, memo));
  }
  
  //Comparing the options
  int res = std::max(X[i] + optionI, X[j] + optionJ);

  //Memoization
  memo[i][j] = res;

  //Final result
  return res;
}



void testcase() {

  //Read input
  int n; std::cin >> n;
  int m; std::cin >> m;
  int p; std::cin >> p;
  VI X(n);
  for (int i = 0; i < n; i++) {
    int val; std::cin >> val;
    X[i] = val;
  }

  //Algorithm
  MI memo(n, VI(n, -1));
  VI possibleWinnings;
  for (int k = 0; k <= p; k++) {
    possibleWinnings.push_back(FRWL(X, k, n-1-p+k, m, memo));
  }
  int solution = *std::min_element(possibleWinnings.begin(), possibleWinnings.end());

  //Print solution
  std::cout << solution << std::endl;

}



int main() {

  std::ios_base::sync_with_stdio(false);



  int t; std::cin >> t;

  for (int i = 0 ; i < t ; i++) {

    testcase();

  }



  return 0;

}
