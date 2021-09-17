#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;


void testcase() {

	//Read input
	int n; std::cin >> n;
	VI A;

	
	for (int i = 0; i < n; i++) {
		int val; std::cin >> val;
		A.push_back(val);
	}

	//Algorithm
	VI S;			// Partial sums
	int E = 0, O = 0;	// Even sums and Odd sums
	S.reserve(n);

	S.push_back(A[0]);
	if (A[0]%2==0) E++;
	else O++;

	for(int i = 1; i < n; i++) {
		S.push_back(S[i-1] + A[i]);
		if (S[i]%2==0) E++;
		else O++;
	}
	int result = E*(E-1)/2 + O*(O-1)/2 + E;

	//Print solution!
	std::cout << result << std::endl;
}


int main() {
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	
	return 0;
}
