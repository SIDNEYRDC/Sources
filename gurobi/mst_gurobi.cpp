/*
*	Minimal Spanning Tree Algorithm with Integer Programming solved by Gorubi©
*	Write by Sidney RDC, 2014.
*/
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <armadillo>
#include "/opt/gurobi563/linux64/include/gurobi_c++.h"

#define MIN_ERR 1e-3

using namespace std;
using namespace arma;

typedef struct edge {
	unsigned int i;	// i vertice
	unsigned int j;	// j vertice
	double w;		// weight of edge
} edge;

// Logic or between two matrices
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

	for(unsigned int id = 0; id < A.n_rows; id++) {
		if(is_critical(A,id)) {
			unsigned int c_num = c_nodes.n_elem;
			c_nodes.resize(c_num+1);
			c_nodes(c_num) = id;
		}
	}

	return c_nodes;
}

int main() {
	// Number of vertices on a graph
	unsigned int n = 6;

	// Weighted adjacency matrix
	mat A = zeros<mat>(n,n);

	A // 0 = inf
	//<< 0 << 4 << 1 << 3 << 0 << 0 << endr
	//<< 4 << 0 << 4 << 4 << 0 << 0 << endr
	//<< 1 << 4 << 0 << 2 << 0 << 0 << endr
	//<< 3 << 4 << 2 << 0 << 4 << 7 << endr
	//<< 0 << 0 << 0 << 4 << 0 << 5 << endr
	//<< 0 << 0 << 0 << 7 << 5 << 0 << endr;
	<< 0 << 3 << 5 << 0 << 0 << 7 << endr
	<< 3 << 0 << 4 << 0 << 0 << 0 << endr
	<< 5 << 4 << 0 << 6 << 0 << 0 << endr
	<< 0 << 0 << 6 << 0 << 1 << 2 << endr
	<< 0 << 0 << 0 << 1 << 0 << 3 << endr
	<< 7 << 0 << 0 << 2 << 3 << 0 << endr;

	uvec c_nodes = critical_nodes(logic_or(A,zeros<mat>(n,n)));
	cout << "c_nodes:\n" << c_nodes << endl;

	// Determine the size of Edges vector: sum(n-i)_{i=1}^{n-1}
	unsigned int n_xe = 0;

	double *coeffs = (double*)malloc(sizeof(double) * 1);
	edge *edges = (edge*)malloc(sizeof(edge) * 1);

	for(unsigned int i = 0; i < n-1; i++) {
		for(unsigned int j = i + 1; j < n; j++) {
			if(A(i,j) > 0) {
				n_xe++;
				coeffs = (double*)realloc(coeffs, sizeof(double) * n_xe);
				coeffs[n_xe-1] = A(i,j);

				edges = (edge*)realloc(edges, sizeof(edge) * n_xe);
				edges[n_xe-1].i = i;
				edges[n_xe-1].j = j;
				edges[n_xe-1].w = A(i,j);
			}
		}
	}

	try {
		GRBEnv env = GRBEnv();

		GRBModel model = GRBModel(env);

		// Hide optimization results
		model.getEnv().set(GRB_IntParam_OutputFlag, 0);

		// Create variables
		double *lb = new double[n_xe];
		double *ub = new double[n_xe];
		double *obj = new double[n_xe];
		char *type = new char[n_xe];
		string *names = new string[n_xe];
		double *coef1 = new double[n_xe];
		for(unsigned int i = 0; i < n_xe; i++) {
			lb[i] = 0;
			ub[i] = 1;
			obj[i] = 0;
			type[i] = GRB_BINARY;
			stringstream ss; ss << "x" << edges[i].i << edges[i].j;
			names[i] = ss.str();
			coef1[i] = 1;
		}

		// Add variables to model
		GRBVar* Xe = model.addVars(lb,ub,obj,type,names,n_xe);

		// Integrate new variables
		model.update();

		// Make the objective linear expression
		GRBLinExpr objExpr;
		objExpr.addTerms(coeffs, Xe, n_xe);

		// Set objective: minimize w0*x0 + w1*x1 + ... + wn*xn
		model.setObjective(objExpr,GRB_MINIMIZE);

		// Make the 1st constraint
		GRBLinExpr c0Expr;
		c0Expr.addTerms(coef1,Xe,n_xe);
		model.addConstr(c0Expr,GRB_EQUAL,n-1,"c0");

		// Dynamic linear expressions for make constraints
		GRBLinExpr *lhsExprs = new GRBLinExpr[2*n];
		char *senses = new char[2*n];
		double *rhsVals = new double[2*n];
		names = new string[2*n];

		// Make the 2nd and 3rd type of constraints
		unsigned int e;
		for(unsigned int v = 0; v < n; v++) {
			e = 0;
			while(e < n_xe) {
				if(edges[e].i == v || edges[e].j == v) {
					lhsExprs[v] += Xe[e];
					lhsExprs[n+v] += Xe[e];
				}
				e++;
			}

			// Make the constraint of type x0 + x1 + ... + xn >= 1
			senses[v] = GRB_GREATER_EQUAL;
			rhsVals[v] = 1;
			stringstream ss; ss << "c" << v+1;
			names[v] = ss.str();

			// Make the constraint of type x0 + x1 + ... + xn <= 2
			senses[n+v] = GRB_LESS_EQUAL;
			rhsVals[n+v] = 2;
			ss << "c" << v+n+1;
			names[n+v] = ss.str();
		}

		// Add constraint to model
		model.addConstrs(lhsExprs,senses,rhsVals,names,2*n);


		//lhsExprs = new GRBLinExpr[c_nodes.n_elem];
		//senses = new char[c_nodes.n_elem];
		//rhsVals = new double[c_nodes.n_elem];
		//names = new string[c_nodes.n_elem];

		// Critical nodes constraints
		//if(!c_nodes.is_empty()) {
			//for(unsigned int c = 0; c < c_nodes.n_elem; c++) {
				//e = 0;
				//while(e < n_xe) {
					//if(edges[e].i == c_nodes(c) || edges[e].j == c_nodes(c)) {
						//lhsExprs[c] += Xe[e];
					//}
					//e++;
				//}

				//// Make the constraint of type x0 + x1 + ... + xn >= 1
				//senses[c] = GRB_GREATER_EQUAL;
				//rhsVals[c] = 2;
				//stringstream ss; ss << "cnc" << c;
				//names[c] = ss.str();
			//}

			//cout << "expr:" << lhsExprs[0] << endl;

			//// Add constraint to model
			//model.addConstrs(lhsExprs,senses,rhsVals,names,c_nodes.n_elem);
		//}

		// Optimize model
		model.optimize();

		A = zeros<mat>(n,n);
		for(unsigned int i = 0; i < n_xe; i++) {
			cout << Xe[i].get(GRB_StringAttr_VarName) << " = " << Xe[i].get(GRB_DoubleAttr_X) << endl;

			if(Xe[i].get(GRB_DoubleAttr_X) > 0) {
				A(edges[i].i,edges[i].j) = A(edges[i].j,edges[i].i) = 1;
			}
		}

		cout << "minA:\n" << A << endl;
		cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	} catch(GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...) {
		cout << "Exception during optimization" << endl;
	}

	return 0;
}

