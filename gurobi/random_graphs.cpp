#include <iostream>
#include <cstdlib>
#include <armadillo>
#include <time.h>
#include <bitset>
#include <cmath>

#define MIN_ERR 1e-3

using namespace std;
using namespace arma;

// Generate a random adjacency matrix
mat rand_adjacency(unsigned int v_n, unsigned int e_n) {
	// Check if the edges number is feasible
	if(e_n < v_n - 1 || e_n > (v_n*(v_n - 1)/2)) {
		cout << "ERROR: The |e| must be: |v|-1 <= |e| <= |v|*(|v|-1)/2" << endl;
		exit(1);
	}

	// Edges counter
	unsigned int e_c;

	// Laplacian matrix
	mat L;

	// Eigen values vector
	cx_vec eigval = zeros<cx_vec>(2);

	// Adjacency matrix
	mat A = zeros<mat>(v_n,v_n);

	cout << "Generating a random connected graph..." << endl;

	// Random seeder
	srand(time(NULL));

	// Generate a connected graph
	while(eigval(1).real() <= MIN_ERR) {
		e_c = 0;
		A = zeros<mat>(v_n,v_n);

		// Make the minimal connected graph
		for(unsigned int i = 0; i < v_n; i++) {
			unsigned int j = random() % v_n;

			uvec N = find(A.col(i));
			if(i != j && N.n_elem <= 2 && !A(i,j) && !A(j,i)) {
				A(i,j) = A(j,i) = 1;
				e_c++;
			}

			if(e_c >= e_n) break;
		}

		// Make the other edges
		while(e_c < e_n) {
			unsigned int j = 0, i = 0;

			while(i == j) {
				i = rand() % v_n;
				j = rand() % v_n;
			}

			if(!A(i,j) && !A(j,i)) {
				A(i,j) = A(j,i) = 1;
				e_c++;
			}
		}

		// Verify if the graph is connected
		L = diagmat(sum(A)) - A;
		eigval = sort(eig_gen(L));

		cout << "l2:" << eigval(1).real() << endl;
	}

	return A;
}

// Compute the time between two times
double calc_time(clock_t t0, clock_t t1){
	return ((double)(t1 - t0) / CLOCKS_PER_SEC);
}

int main() {
	unsigned int n = 5;	// Number of vertices
	unsigned int e_n = 4;	// Nubmer of edges


	// Time counter variables
	clock_t t0, t1;

	// Get initial time
	t0 = clock();

	mat A = rand_adjacency(n,e_n);

	// Get final time
	t1 = clock();

	cout << "A:\n" << A << endl;
	cout << "time:" << calc_time(t0,t1) << "s" << endl;

	mat range = ones<mat>(n,2);
	mat pose = zeros<mat>(n,2);

	range.col(0) *= 1;		// Maximum range
	range.col(1) *= 0.5;	// Minimum range

	pose(0,0) = 0;	// x=0
	pose(0,1) = 0;	// y=0

	//float HI = 1;
	//float LO = 0.2;
	//srand(static_cast<unsigned>(time(0)));
	//float r3 = LO + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(HI-LO)));

	//cout << "flot:" << r3;

	srand(static_cast<unsigned>(time(0)));
	for(unsigned int i = 0; i < n; i++) {
		uvec N = find(A.col(i));
		float HI = range(i,1), LO = range(i,0);
		float dist = LO + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(HI-LO)));

		for(unsigned int j = 0; j < N.n_elem; j++) {
			float x, y = static_cast<float>(rand())/(static_cast<float>(RAND_MAX/dist));
			x = sqrt(pow(dist,2) + pow(y,2));

			cout << "x:" << x << " y:" << y << " dist:" << dist << endl;

			pose(N(j),0) = 0;
		}
	}


	return 0;
}
