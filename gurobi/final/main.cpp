#include <iostream>
#include "k_optimization.hpp"
#include "graph_utils.hpp"

using namespace std;
using namespace arma;

int main() {
	// Weighted adjacency matrix
	mat A = zeros<mat>(6,6);

	A
	//<< 0 << 3 << 5 << 0 << 0 << 10 << endr
	//<< 3 << 0 << 4 << 0 << 0 << 0 << endr
	//<< 5 << 4 << 0 << 6 << 0 << 0 << endr
	//<< 0 << 0 << 6 << 0 << 1 << 2 << endr
	//<< 0 << 0 << 0 << 1 << 0 << 3 << endr
	//<< 10 << 0 << 0 << 2 << 3 << 0 << endr;
	<< 0 << 2 << 0 << 0 << 4 << endr
	<< 2 << 0 << 7 << 0 << 1 << endr
	<< 0 << 7 << 0 << 1 << 3 << endr
	<< 0 << 0 << 1 << 0 << 2 << endr
	<< 4 << 1 << 3 << 2 << 0 << endr;

	//cout << mst_optimization(A);
	cout << "A:\n" << A << "\n2-minA:\n" << k2_optimization(A) << endl;

	return 0;
}
