#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;

#define MAX_COORD 1000000

void testcase() {
	//Read input
	int n; std::cin >> n;
	VI positions;
	VI parasols(MAX_COORD*2 + 1, 0);

	for (int i = 0; i < n; i++) {
		int val; std::cin >> val;
		val += MAX_COORD;
		positions.push_back(val);
		parasols[val] = 1;
	}
	std::sort(positions.begin(), positions.end());

	//Algorithm
	int i = -100;
	int j = 100;
	int numParasols = 0;
	int max_dist_R;
	int max_dist_L = - positions[0];
	int max_dist_L_index = 0;

	VI optimal_points;
	int maxParasols = 0;
	int min_longest_distance = 0;

	//Initialization
	optimal_points.push_back(0);
	for (int k = 0; k <= j; k++) {
		if (parasols[k]) {
			numParasols++;
			min_longest_distance = k;
			max_dist_R = k;
		}
	}

	//Sliding the window
	while ((i+j)/2 <= positions[n-1]) {
		if (i >= 0 && parasols[i] == 1) {
			numParasols--;
			max_dist_L_index++;
			max_dist_L = (i+j)/2 + 1 - positions[max_dist_L_index];
		}
		else if (i >= 0 && parasols[i] == 0) {
			max_dist_L++;
		}
		i++;
		j++;
		if (j <= 2*MAX_COORD && parasols[j] == 1) {
			numParasols++;
			max_dist_R = (j-i)/2; // j - (i+j)/2
		} else {
			max_dist_R--;
		}

		int longest_distance = std::max(max_dist_L, max_dist_R);
		if (numParasols > maxParasols or
			(numParasols == maxParasols and longest_distance < min_longest_distance)
		   ) {
			maxParasols = numParasols;
			min_longest_distance = longest_distance;
			optimal_points.clear();
			optimal_points.push_back((i+j)/2);
		}
		else if (numParasols == maxParasols && longest_distance == min_longest_distance) {
			optimal_points.push_back((i+j)/2);
		}
	}


	//Print solution!
	std::cout << maxParasols << " " << min_longest_distance << std::endl;
	for (int sol : optimal_points) {
		std::cout << sol - MAX_COORD << " ";
	}
	std::cout << std::endl;

}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}

	return 0;
}
