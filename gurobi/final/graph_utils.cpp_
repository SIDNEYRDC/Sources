#include "graph_utils.hpp"

using namespace std;
using namespace arma;

// Logic or between two adjacency matrices
mat logic_or(mat x, mat y) {
	mat out = x + y;

	for(unsigned int i=0; i<out.n_rows; i++) {
		for(unsigned int j=0; j<out.n_cols; j++) {
			if(out(i,j) > 1) out(i,j) = 1;
		}
	}

	return out;
}

// Reachability for an adjacency matrix A
mat reach_matrix(mat A) {
	mat r = logic_or(A,eye(A.n_rows,A.n_cols));
	mat aux = zeros(A.n_rows,A.n_cols);

	while(prod(prod(aux == r)) == 0) {
		aux = r;
		r = logic_or(r,(r*r));
	}

	return r;
}

// Reduce the adjacency matrix to a reachable neighbors adjacency matrix
std::vector<mat> min_mat(int id, mat A) {
	unsigned int count = 0;
	std::vector<mat> out(2);

	// Reachability matrix
	mat r_mat = reach_matrix(A);

	mat index = zeros<mat>(1,A.n_rows);
	index.fill(-1);

	for(unsigned int j = 0; j < A.n_rows; j++) {
		if(r_mat(id,j) || r_mat(j,id)) {
			index(0,count) = j;
			count++;
		}
	}

	index.resize(1,count);

	mat min_A = zeros<mat>(count,count);

	for(unsigned int i = 0; i < count-1; i++) {
		for(unsigned int j = i+1; j < count; j++) {
			min_A(i,j) = A(index(0,i),index(0,j));
			min_A(j,i) = A(index(0,j),index(0,i));
		}
	}

	if(count > 1) {
		out[0] = min_A;
		out[1] = index;
	}

	return out;
}

// Verify if a node is critical
bool is_critical(mat A, unsigned int node) {
	// Reduce the adjacency matrix to the reachable nodes only
	std::vector<mat> out = min_mat(node,A);

	// The node is alone
	if(out[0].is_empty()) return false;

	// Real id for the minimized adjacency matrix
	uvec real_id = find(out[1].row(0) == node);

	// Remove the node from the graph
	mat test_A;
	if(real_id(0) == 0) {
		test_A = out[0].submat(1,1,out[0].n_rows-1,out[0].n_rows-1);
	} else if(real_id(0) == out[0].n_rows-1) {
		test_A = out[0].submat(0,0,out[0].n_rows-2,out[0].n_rows-2);
	} else {
		test_A = out[0].submat(0,0,real_id(0)-1,real_id(0)-1);
		test_A.resize(out[0].n_rows-1,out[0].n_rows-1);
		test_A.submat(real_id(0),0,out[0].n_rows-2,real_id(0)-1) = out[0].submat(real_id(0)+1,0,out[0].n_rows-1,real_id(0)-1);
		test_A.submat(0,real_id(0),real_id(0)-1,out[0].n_rows-2) = out[0].submat(0,real_id(0)+1,real_id(0)-1,out[0].n_rows-1);
		test_A.submat(real_id(0),real_id(0),out[0].n_rows-2,out[0].n_rows-2) = out[0].submat(real_id(0)+1,real_id(0)+1,out[0].n_rows-1,out[0].n_rows-1);
	}

	// Verify if the node is critical
	mat L = diagmat(sum(test_A)) - test_A;
	cx_vec eigval = sort(eig_gen(L));

	// If 2nd eigenvalue is zero, the node is critical
	if(eigval(1).real() < MIN_ERR) return true;
	else return false;
}

// Find all critical nodes in the graph
uvec critical_nodes(mat A) {
	uvec c_nodes;
	unsigned int n = A.n_rows, c_num;

	for(unsigned int id = 0; id < n; id++) {
		if(is_critical(A,id)) {
			c_num = c_nodes.n_elem;
			c_nodes.resize(c_num+1);
			c_nodes(c_num) = id;
		}
	}

	return c_nodes;
}
