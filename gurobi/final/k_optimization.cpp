#include "k_optimization.hpp"

using namespace std;
using namespace arma;

// Enumerate all connected subgraphs on a graph
void subgraph(umat &S, vector<int> sg, mat A, uvec N, unsigned int depth, int &id_s, int id_sg) {
	for(unsigned int i = 0; i < N.n_elem; i++) {
		// If neighbor i not exist in sg
		if(find(sg.begin(), sg.end(), N(i)) == sg.end()) {
			// The subgraph set
			sg[id_sg] = N(i);

			// The neighbor of actual node set
			uvec N_j = find(A.col(N(i)));

			// Test the function reach the top of stack
			if(depth > 1) subgraph(S,sg,A,N_j,depth-1,id_s,id_sg+1);
			else {
				float sum = 0, sum2 = 0;
				bool exist = false;

				// The sum of squares for actual subgraph
				for(unsigned int k = 0; k < S.n_cols; k++) {
					sum += pow(sg[k],2);
				}

				// The sum of squares for all generated subgraphs
				for(unsigned int r = 0; r < S.n_rows; r++) {
					sum2 = 0;
					for(unsigned int k = 0; k < S.n_cols; k++) {
						sum2 += pow(S(r,k),2);
					}
					if(sum2 == sum) {
						exist = true;
						break;
					}
				}

				// If the actual subgraph not exist in S, save it
				if(!exist) {
					id_s++;
					S.resize(id_s+1,S.n_cols);

					for(unsigned int k = 0; k < S.n_cols; k++) {
						S(id_s,k) = sg[k];
					}
				}
			}
		}
	}
}

// Overload of subgraph generation
void subgraph(umat &S, mat A, unsigned int depth) {
	unsigned int n = A.n_rows;
	int id_s = -1, id_sg = 0;
	vector<int> sg(S.n_cols);

	// For all nodes, generate the connected subgraphs
	for(unsigned int i = 0; i < n; i++) {
		sg[id_sg] = i;
		uvec N = find(A.col(i));
		subgraph(S,sg,A,N,depth-1,id_s,id_sg+1);
	}
}

// Generate a Minimal Spanning Tree for a weighted adjacency matrix A
mat mst_optimization(mat A) {
	// Number of nodes in the graph
	unsigned int n = A.n_rows;

	// Output matrix
	mat out = zeros<mat>(n,n);

	// Determine the size of Edges vector: sum(n-i)_{i=1}^{n-1}
	unsigned int n_xe = 0;

	// Edges and weights arrays
	double *coeffs = (double*)malloc(sizeof(double) * 1);
	edge *edges = (edge*)malloc(sizeof(edge) * 1);

	// Make edges with its weights
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
			} else if(A(j,i) > 0) {
				n_xe++;
				coeffs = (double*)realloc(coeffs, sizeof(double) * n_xe);
				coeffs[n_xe-1] = A(j,i);

				edges = (edge*)realloc(edges, sizeof(edge) * n_xe);
				edges[n_xe-1].i = j;
				edges[n_xe-1].j = i;
				edges[n_xe-1].w = A(j,i);
			}
		}
	}

	// Test if MST can be done
	if(n_xe < n) return A;

	try {
		// Gurobi Optimization environment and model
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		// Hide optimization results
		model.getEnv().set(GRB_IntParam_OutputFlag, 0);

		// Create and fill variables
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

		// Make the constraint: x0 + x1 + .. + xn = n - 1
		GRBLinExpr c0Expr;
		c0Expr.addTerms(coef1,Xe,n_xe);
		model.addConstr(c0Expr,GRB_EQUAL,n-1,"c0");

		// Make the constraint: x0 + x1 + ... + xi <= |S| - 1 for all S \in V
		for(unsigned int depth = 3; depth <= n / 2; depth++) {
			// Subgraphs set
			umat S = zeros<umat>(1,depth);

			// Generate all connected subgraphs with size = depth
			subgraph(S,A,depth);

			// Constraint number
			unsigned int n_c = S.n_rows;

			// Dynamic linear expressions for make constraints
			GRBLinExpr *lhsExprs = new GRBLinExpr[n_c];
			char *senses = new char[n_c];
			double *rhsVals = new double[n_c];
			names = new string[n_c];
			stringstream ss;

			for(unsigned int c = 0; c < n_c; c++) {
				for(unsigned int v1 = 0; v1 < depth-1; v1++) {
					for(unsigned int v2 = v1+1; v2 < depth; v2++) {
						for(unsigned int e = 0; e < n_xe; e++) {
							if((edges[e].i == S(c,v1) && edges[e].j == S(c,v2)) || (edges[e].j == S(c,v1) && edges[e].i == S(c,v2))) {
								lhsExprs[c] += Xe[e];
							}
						}
					}
				}

				senses[c] = GRB_LESS_EQUAL;
				rhsVals[c] = depth - 1;
				ss << "c" << c << "-" << depth;
				names[c] = ss.str();
			}

			// Add constraint to model
			model.addConstrs(lhsExprs,senses,rhsVals,names,n_c);
		}

		// Optimize model
		model.optimize();

		// Make the MST weighted adjacency matrix
		for(unsigned int i = 0; i < n_xe; i++) {
			if(Xe[i].get(GRB_DoubleAttr_X) > 0) {
				if(A(edges[i].i,edges[i].j) && A(edges[i].j,edges[i].i))
					out(edges[i].i,edges[i].j) = out(edges[i].j,edges[i].i) = edges[i].w;
				else if(A(edges[i].i,edges[i].j))
					out(edges[i].i,edges[i].j) = edges[i].w;
				else if(A(edges[i].j,edges[i].i))
					out(edges[i].j,edges[i].i) = edges[i].w;
			}
		}

	} catch(GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...) {
		cout << "Exception during optimization" << endl;
	}

	return out;
}

// Generate a bi-connected Minimal Spanning Tree for a weighted adjacency matrix A
mat k2_optimization(mat A) {
	// Number of nodes in the graph
	unsigned int n = A.n_rows;

	// Output matrix
	mat out = zeros<mat>(n,n);

	// Determine the size of Edges vector: sum(n-i)_{i=1}^{n-1}
	unsigned int n_xe = 0;

	// Edges and weights arrays
	double *coeffs = (double*)malloc(sizeof(double) * 1);
	edge *edges = (edge*)malloc(sizeof(edge) * 1);

	// Make edges with its weights
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
			} else if(A(j,i) > 0) {
				n_xe++;
				coeffs = (double*)realloc(coeffs, sizeof(double) * n_xe);
				coeffs[n_xe-1] = A(j,i);

				edges = (edge*)realloc(edges, sizeof(edge) * n_xe);
				edges[n_xe-1].i = j;
				edges[n_xe-1].j = i;
				edges[n_xe-1].w = A(j,i);
			}
		}
	}

	// Test if 2-MST can be done
	if(n_xe <= n) return A;

	try {
		// Gurobi Optimization environment and model
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);

		// Hide optimization results
		model.getEnv().set(GRB_IntParam_OutputFlag, 0);

		// Create and fill variables
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

		// Make the constraint: x0 + x1 + .. + xn = n
		GRBLinExpr c0Expr;
		c0Expr.addTerms(coef1,Xe,n_xe);
		model.addConstr(c0Expr,GRB_EQUAL,n,"c0");

		// Dynamic linear expressions for make constraints
		GRBLinExpr *lhsExprs = new GRBLinExpr[n];
		char *senses = new char[n];
		double *rhsVals = new double[n];
		names = new string[n];
		stringstream ss;

		// Make the constraint: xi0 + xi1 .. + xin = 2 for all i \in V
		for(unsigned int v = 0; v < n; v++) {
			for(unsigned int e = 0; e < n_xe; e++) {
				if(edges[e].i == v || edges[e].j == v) {
					lhsExprs[v] += Xe[e];
				}
			}

			senses[v] = GRB_EQUAL;
			rhsVals[v] = 2;
			ss << "c" << v+1;
			names[v] = ss.str();
		}

		// Add constraint to model
		model.addConstrs(lhsExprs,senses,rhsVals,names,n);

		// Optimize model
		model.optimize();

		// Make the 2-MST weighted adjacency matrix
		for(unsigned int i = 0; i < n_xe; i++) {
			if(Xe[i].get(GRB_DoubleAttr_X) > 0) {
				if(A(edges[i].i,edges[i].j) && A(edges[i].j,edges[i].i))
					out(edges[i].i,edges[i].j) = out(edges[i].j,edges[i].i) = edges[i].w;
				else if(A(edges[i].i,edges[i].j))
					out(edges[i].i,edges[i].j) = edges[i].w;
				else if(A(edges[i].j,edges[i].i))
					out(edges[i].j,edges[i].i) = edges[i].w;
			}
		}

	} catch(GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...) {
		cout << "Exception during optimization" << endl;
	}

	return out;
}
