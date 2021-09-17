#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;


int computeDistance(int start, bool ownerIsMoving, const MI& ADJS, MI& memo) {
	int n = ADJS.size() - 1;
	//Base case
	if (start == n) return 0;

	//Recall
	if (ownerIsMoving && memo[0][start] != -1) return memo[0][start];
	else if (!ownerIsMoving && memo[1][start] != -1) return memo[1][start];

	//Recursive computation
	int res = 1 + computeDistance(ADJS[start][0], !ownerIsMoving, ADJS, memo);

	for (auto it = ADJS[start].cbegin()++; it != ADJS[start].cend(); it++) {
		int val = 1 + computeDistance(*it, !ownerIsMoving, ADJS, memo);
		if (ownerIsMoving) res = std::min(res, val);
		else res = std::max(res, val);
	}


	//Memoization
	memo[!ownerIsMoving][start] = res;

	//Return result
	return res;
}

char whoWins(int rDist, int bDist) {
	int valToSubtract = std::min(rDist, bDist);
	if (valToSubtract%2 != 0) valToSubtract--;

	rDist -= valToSubtract;
	bDist -= valToSubtract;

	if (bDist == 0) return 'm';
	if (rDist == 0) return 's';
	rDist--;
	if (rDist == 0) return 's';
	bDist--;
	if (bDist == 0) return 'm';
	else {
		std::cout << "This case should be impossible" << std::endl;
		exit(1);
	}
}
	

void testcase() {
	//Read input
	int n; std::cin >> n;
	int m; std::cin >> m;
	int r; std::cin >> r;
	int b; std::cin >> b;

	MI ADJS(n+1);
	for (int i = 0; i < m; i++) {
		int u; std::cin >> u;
		int v; std::cin >> v;
		ADJS[u].push_back(v);
	}


	//Algorithm
	MI memo(2, VI(n+1, -1));
	/*
	Structure of the memo:
	We have two arrays of length n+1.
	memo[0][i] := ensured 'time' (i.e. number of turns) to reach n from i
	memo[1][i] := maximum 'time' to reach n from i
	*/
	
	int distanceForRed = computeDistance(r, true, ADJS, memo);
	int distanceForBlack = computeDistance(b, true, ADJS, memo);
	char winner = whoWins(distanceForRed, distanceForBlack);
	int solution = 0;
	if (winner != 's') solution = 1;

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
	
