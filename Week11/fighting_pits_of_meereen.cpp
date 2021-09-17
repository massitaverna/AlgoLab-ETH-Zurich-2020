#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#define MAX_DIFF 12
#define ABORT -1

typedef std::vector<int> VI;
typedef std::vector<VI> MI;
typedef std::vector<MI> M3I;
typedef std::vector<M3I> M4I;
typedef std::vector<bool> VB;

int toInt(const VI& lastOnes, int k) {
  //m-1 elements with k+1 different values
  int m = lastOnes.size() + 1;
  int a = 1;
  int result = 0;
  for (int i = 0; i < m-1; i++) {
    result += a*(lastOnes[i]+1);
    a *= k+1;
  }
  return result;
}

int f(const VI& types, int k, int sentSoFar, int sentToN, VI lastN, VI lastS, M4I& memo) {
  int m = lastN.size() + 1;
  
  // Recall old computations
  if (memo[sentSoFar][sentSoFar-2*sentToN+MAX_DIFF][toInt(lastN, k)][toInt(lastS, k)] != -1) {
    return memo[sentSoFar][sentSoFar-2*sentToN+MAX_DIFF][toInt(lastN, k)][toInt(lastS, k)];
  }
  
  // Send fighter to N...
  VB countTypesN(k, false);
  countTypesN[types[sentSoFar]] = true;
  for (int type : lastN) {
    if (type != -1) {
      countTypesN[type] = true;
    }
  }
  int differentTypes = 0;
  for (bool isPresent : countTypesN) {
    if (isPresent) differentTypes++;
  }
  int pointsN = 1000*differentTypes;
  pointsN -= 1 << std::abs(sentSoFar - 2*sentToN - 1);
  
  // ... or send fighter to S
  VB countTypesS(k, false);
  countTypesS[types[sentSoFar]] = true;
  for (int type : lastS) {
    if (type != -1) {
      countTypesS[type] = true;
    }
  }
  differentTypes = 0;
  for (bool isPresent : countTypesS) {
    if (isPresent) differentTypes++;
  }
  int pointsS = 1000*differentTypes;
  pointsS -= 1 << std::abs(sentSoFar - 2*sentToN + 1);
  
  // Base case
  if ((unsigned int) sentSoFar == types.size()-1) {
    return std::max(pointsN, pointsS);
  }
  
  // New computation: recursive case
  VI newLastN = lastN;
  VI newLastS = lastS;
  int updatePosition;
  for (updatePosition = 0; updatePosition < m-1; updatePosition++) {
    if (lastN[updatePosition] == -1) break;
  }
  if (updatePosition == m-1) {
    for (int i = 0; i < m-2; i++) {
      newLastN[i] = newLastN[i+1];
    }
    updatePosition = m-2;
  }
  newLastN[updatePosition] = types[sentSoFar];
  for (updatePosition = 0; updatePosition < m-1; updatePosition++) {
    if (lastS[updatePosition] == -1) break;
  }
  if (updatePosition == m-1) {
    for (int i = 0; i < m-2; i++) {
      newLastS[i] = newLastS[i+1];
    }
    updatePosition = m-2;
  }
  newLastS[updatePosition] = types[sentSoFar];
  int valN, valS;
  if (pointsN < 0) valN = ABORT;
  else valN = f(types, k, sentSoFar+1, sentToN+1, newLastN, lastS, memo);
  if (pointsS < 0) valS = ABORT;
  else valS = f(types, k, sentSoFar+1, sentToN, lastN, newLastS, memo);
  int result;
  if (valN == ABORT) {
    if (valS != ABORT)  result = valS + pointsS;
    else                result = ABORT;
  }
  else {
    if (valS == ABORT)  result = valN + pointsN;
    else                result = std::max(valN + pointsN,valS + pointsS);
  }

  // Memoization
  memo[sentSoFar][sentSoFar-2*sentToN+MAX_DIFF][toInt(lastN, k)][toInt(lastS, k)] = result;
  
  return result;
}

void testcase() {
  // Read input
  int n, k, m; std::cin >> n >> k >> m;
  VI lastN(m-1, -1);
  VI lastS(m-1, -1);
  VI types(n);
  
  for (int i = 0; i < n; i++) {
    std::cin >> types[i];
  }
  
  // Algorithm
  int lastOnesSize = std::pow(k+1, m-1);
  M4I memo(n, M3I(2*MAX_DIFF, MI(lastOnesSize, VI(lastOnesSize, -1))));
  int solution = f(types, k, 0, 0, lastN, lastS, memo);

  // Print solution!
  std::cout << solution << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) testcase();
}
