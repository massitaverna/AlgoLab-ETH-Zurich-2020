#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

#define MAX_POSITION 0x40000000

typedef std::vector<int> VI;
typedef std::vector<VI> MI;


void testcase() {
	//Read input
	int n; std::cin >> n;
	VI m(n);
	MI P(n);
	for (int i = 0; i < n; i++) {
		int val; std::cin >> val;
		m[i] = val;
	}
	for (int i = 0; i < n; i++) {
		VI temp(m[i]);
		for (int j = 0; j < m[i]; j++) {
			int val; std::cin >> val;
			temp[j] = val;
		}
		P[i] = temp;
	}

	//Algorithm
	VI indeces(n, 0);
	std::map<int, int> sorted_positions;
	int min = MAX_POSITION + 1;
	int max = -1;
	int idx_min; // ID number of first word in snippet
	int idx_max; // ID number of last  word in snippet

	//Initialization
	for (int i = 0; i < n; i++) {
		P[i][0];
		if (P[i][0] < min) {
			min = P[i][0];
			idx_min = i;
		}
		if (P[i][0] > max) {
			max = P[i][0];
			idx_max = i;
		}
		for (int j = 0; j < m[i]; j++) {
			sorted_positions.insert(std::make_pair(P[i][j], i));
		}
	}
	int a = min;
	int b = max;
	auto pointer_to_min = sorted_positions.cbegin();

	//Sliding the window
	while(indeces[idx_min] < m[idx_min] - 1) {
		indeces[idx_min]++;
		int min_successor = P[idx_min][indeces[idx_min]];
		if (min_successor > max) {
			max = min_successor;
			idx_max = idx_min;
		}
		pointer_to_min++;
		min = pointer_to_min->first;
		idx_min = pointer_to_min->second;

		if (max - min < b - a) {
			a = min;
			b = max;
		}
	}

	//Print solution!
	std::cout << b - a + 1 << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}

	return 0;
}
