#include <iostream>
#include <vector>

typedef std::vector<int> VI;
typedef std::vector<VI> MI;

void print_matrix(MI A, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << A[i][j] << " ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void testcase() {

	//Read input
	int n; std::cin >> n;
	MI A(n, VI(n));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int val; std::cin >> val;
			A[i][j] = val;
		}
	}

	//Algorithm
	MI S;	// Matrix of partial sums

	//Building S
	VI P0;
	P0.push_back(A[0][0]);
	for (int k = 1; k < n; k++) {
		P0.push_back(P0[k-1] + A[0][k]);
	}
	S.push_back(P0);

	for (int i = 1; i < n ; i++) {
		VI row;
		VI P;
		VI previous = S.back();
		P.push_back(A[i][0]);
		row.push_back(previous[0] + P[0]);
		for (int j = 1; j < n ; j++) {
			P.push_back(P[j-1] + A[i][j]);
			row.push_back(previous[j] + P[j]);
		}
		S.push_back(row);
	}


	/*
	// O(n^4) solution
	int counter = 0;
	for (int i1 = 0; i1 < n; i1++) {
		for (int j1 = 0; j1 < n; j1++) {
			for (int i2 = i1; i2 < n; i2++) {
				for (int j2 = j1; j2 < n; j2++) {
					int value;
					if (i1 > 0 && j1 > 0)
						value = S[i2][j2] - S[i1-1][j2] - S[i2][j1-1] + S[i1-1][j1-1];
					else if (i1 > 0)
						value = S[i2][j2] - S[i1-1][j2];
					else if (j1 > 0)
						value = S[i2][j2] - S[i2][j1-1];
					else
						value = S[i2][j2];

					if (value%2 == 0) counter++;
				}
			}
		}
	}
	*/



	// O(n^3) solution

	MI EE(n, VI(n, -1)); //(i,j) = # of 'even-even' pairs in the same row in columns i and j
	MI OO(n, VI(n, -1)); //(i,j) = # of 'odd-odd' pairs ...
	MI EO(n, VI(n, -1)); //(i,j) = # of 'even-odd' or 'odd-even' pairs ...
	
	//Building EE, OO and EO
	for (int i = 0; i < n; i++) {
		for (int j = i; j < n; j++) {
			int ee = 0, oo = 0, eo = 0;
			for (int k = 0; k < n; k++) {
				if (S[k][i]%2 == 0 && S[k][j]%2==0) ee++;
				else if (S[k][i]%2 != 0 && S[k][j]%2!=0) oo++;
				else eo++;
			}
			EE[i][j] = ee;
			OO[i][j] = oo;
			EO[i][j] = eo;
		}
	}
	
	//Finding a solution
	int result = 0;
	for (int i = 0; i < n; i++) {
		result -= OO[i][i];		// Subtracted since it will be added in the following loop,
						// but actually this term must not be added

		result -= EE[i][i]*OO[i][i];	// Same reason

		for (int j = i; j < n; j++) {
			int v = EE[i][j];
			result += v*(v-1)/2 + v;
			v = OO[i][j];
			result += v*(v-1)/2 + v;
			v = EO[i][j];
			result += v*(v-1)/2;
			result += EE[i][j]*OO[i][j];
		}
	}


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
