#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;

struct Sequence {
	int start;
	int end;
	int next = -1;
	Sequence(int s, int e) : start(s), end(e) {}
};


int f(int i, int k, const std::vector<Sequence>& sequences, MI& memo) {
	if (i >= sequences.size() and k > 0) return -1;
	if (k == 0) return 0;

	if (memo[i][k] != -2) {
		return memo[i][k];
	}

	int a = sequences[i].end - sequences[i].start + 1;
	if (sequences[i].next != -1 or k == 1) {
		int toAdd = f(sequences[i].next, k-1, sequences, memo);
		a += toAdd;
		if (toAdd == -1) {
			a = -1;
		}
	}
	else {
		a = -1;
	}

	int b = f(i+1, k, sequences, memo);
	int res = std::max(a, b);

	memo[i][k] = res;
	
	return res;
}

void testcase() {
	int n, m, k; std::cin >> n >> m >> k;

	VI values(n);
	for (int i = 0; i < n; i++) {
		std::cin >> values[i];
	}

	std::vector<Sequence> sequences;

	//Sliding Window
	int i = 0;
	int j = 0;
	int sum = 0;

	while (j < n) {
		sum += values[j];
		if (sum == k) {
			sequences.push_back(Sequence(i, j));
		}
		if (sum < k) {
			j++;
		}
		else {
			sum -= values[i];
			sum -= values[j];
			i++;
		}
		if (i > j) {
			j = i;
			sum = 0;
		}
	}

	//Find next non-overlapping sequence for each sequence
	i = 0;
	j = 0;
	while (j < sequences.size()) {
		while (j < sequences.size() and sequences[j].start <= sequences[i].end) j++;
		if (j < sequences.size()) {
			sequences[i].next = j;
			i++;
		}
	}

	//DP solution
	MI memo(n, VI(m+1, -2));
	int result = f(0, m, sequences, memo);

	//Print solution!
	if (sequences.size() != 0 and result != -1) {
		std::cout << result << std::endl;
	} else {
		std::cout << "fail" << std::endl;
	}
}


int main() {
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	while (t--) testcase();
}

