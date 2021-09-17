#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;

int BCChoice(const VI& C, int i, int j, MI& memo);



int BurningCoins(const VI& C, int i, int j, MI& memo) {
  if (i == j) return C[i];
  if (i == j-1) return std::max(C[i], C[j]);

  if (memo[i][j] != 0) return memo[i][j];

  int res;
  int opponentChoice;
  
  //If I choose i:
  opponentChoice = BCChoice(C, i+1, j, memo); // 4:623 
  if (opponentChoice == i+1) res = C[i] + BurningCoins(C, i+2, j, memo);
  else res = C[i] + BurningCoins(C, i+1, j-1, memo); //508

  //If I choose j:
  opponentChoice = BCChoice(C, i, j-1, memo); // 3:498
  if (opponentChoice == i) res = std::max(res, C[j] + BurningCoins(C, i+1, j-1, memo));
  else res = std::max(res, C[j] + BurningCoins(C, i, j-2, memo));


  memo[i][j] = res;
  return res;
}

int BCChoice(const VI& C, int i, int j, MI& memo) {
  if (i == j) return i;
  if (i == j-1 and C[i] < C[j]) return j;
  else if (i == j-1) return i;

  if (BurningCoins(C, i+1, j, memo) < BurningCoins(C, i, j-1, memo)) return i;
  return j;
}

void testcase() {

  //Read input
  int n; std::cin >> n;
  VI C(n+1);
  for (int i = 1; i <= n; i++) {
    int val; std::cin >> val;
    C[i] = val;
  }

  //Algorithm
  MI memo(n+1, VI(n+1, 0));
  int solution = BurningCoins(C, 1, n, memo);

  //Print solution!
  std::cout << solution << std::endl;
    
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }

  return 0;
}
