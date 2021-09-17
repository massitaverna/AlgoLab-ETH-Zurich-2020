#include <iostream>
#include <vector>


//Returns the absolute value of the difference between the parameters
int dist(int a, int b) {
	int result = a - b;
	if (result < 0) return -result;
	return result;
}

void testcase() {
	std::vector<int> A;
	int n; std::cin >> n;
	int k; std::cin >> k;

	// Vector initialization
	for (int c = 0; c < n; c++) {
		int v; std::cin >> v;
		A.push_back(v);
	}

	int i = 0, j = 0;		//current indexes
	int i_opt, j_opt;		//indexes of the optimal solution so far
	int sum = A[0];			//sum of elements from A[i] to A[j]
	int val;			//current distance between sum and the desired k
	int val_opt = 0x7fffffff - 1;	//2^31 - 1, since val <= 2^30 always

	for (i=0;i<n;i++) {
		// Reset correct value of j in case it remained before i
		if (j < i) {
			j = i;	
			sum = A[i];
		}
		val = dist(sum, k);
		if (val < val_opt) {
			val_opt = val;
			i_opt = i;
			j_opt = j;
			if (val == 0) break; //No better solutions can be found
		} else if (sum > k) {	//Current val is not better than val_opt AND sum is already over k:
					//adding other elements won't help.
			sum -= A[i];
			continue;
		}
		while (1) {
			j++;
			if (j == n) {	//End of the vector: cannot add more elements.
				i = n;  //sum < k and I want to find a better solution, BUT there are no more
				break;	//elements, so there are no more solutions to test.
			}
			sum += A[j];
			val = dist(sum, k);
			if (val < val_opt) {
				val_opt = val;
				i_opt = i;
				j_opt = j;
				if (val == 0) { //No better solutions can be found
					i = n;
					break;
				}
			} else if (sum > k) {
				sum -= A[j];	//Resetting j to the optimal j and the sum up to the correct j
				j--;		//also here
				sum -= A[i];	//And here I remove A[i] for the next iteration of the main cycle
				break;
			}
		}
	}
	
	std::cout << i_opt << " " << j_opt << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t; std::cin >> t;
	for (int i = 0 ; i < t ; i++) {
		testcase();
	}

	return 0;
}
