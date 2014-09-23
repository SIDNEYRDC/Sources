/*
*	Minimal Spanning Tree Algorithm with Integer Programming solved by Gorubi©
*	Write by Sidney RDC, 2014.
*/
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <armadillo>
#include "/opt/gurobi563/linux64/include/gurobi_c++.h"

using namespace std;
using namespace arma;

int main(int argc, char** argv) {
	int n = 6;	// Number of vertices on a graph
	//int A[n][n];		// Adjacency matrix
	int* Xe;			// Edges vector
	float* We;			// Cost vector
	
	// Weighted adjacency matrix
	mat A = zeros<mat>(n,n);
	
	A // 0 = inf
	<< 0 << 4 << 1 << 3 << 0 << 0 << endr
	<< 4 << 0 << 4 << 4 << 0 << 0 << endr
	<< 1 << 4 << 0 << 2 << 0 << 0 << endr
	<< 3 << 4 << 2 << 0 << 4 << 7 << endr
	<< 0 << 0 << 0 << 4 << 0 << 5 << endr
	<< 0 << 0 << 0 << 7 << 5 << 0 << endr;
	
	// Determine the size of Edges vector: sum(n-i)_{i=1}^{n-1}
	unsigned int n_xe = 0;
	for(int i = 1; i < n; i++) n_xe += n - i;
	
	// Set size of edges vector and cost vector
	Xe = new int[n_xe];
	We = new float[n_xe];
	
	cout << "n_xe:" << n_xe << endl;
	
	Xe[0] = 1;	// A[1][0] - x0
	Xe[1] = 1;	// A[2][0] - x1
	Xe[2] = 1;	// A[2][1] - x2
	Xe[3] = 1;	// A[3][0] - x3
	Xe[4] = 1;	// A[3][1] - x4
	Xe[5] = 1;	// A[3][2] - x5
	Xe[6] = 0;	// A[4][0]
	Xe[7] = 0;	// A[4][1]
	Xe[8] = 0;	// A[4][2]
	Xe[9] = 1;	// A[4][3] - x6
	Xe[10] = 0;	// A[5][0]
	Xe[11] = 0;	// A[5][1]
	Xe[12] = 0;	// A[5][2]
	Xe[13] = 1;	// A[5][3] - x7
	Xe[14] = 1;	// A[5][4] - x8
	
	We[0] = 4;	// cost(A[1][0]) - x0
	We[1] = 1;	// cost(A[2][0]) - x1
	We[2] = 4;	// cost(A[2][1]) - x2
	We[3] = 3;	// cost(A[3][0]) - x3
	We[4] = 4;	// cost(A[3][1]) - x4
	We[5] = 2;	// cost(A[3][2]) - x5
	We[6] = 0;	// cost(A[4][0])
	We[7] = 0;	// cost(A[4][1])
	We[8] = 0;	// cost(A[4][2])
	We[9] = 4;	// cost(A[4][3]) - x6
	We[10] = 0;	// cost(A[5][0])
	We[11] = 0;	// cost(A[5][1])
	We[12] = 0;	// cost(A[5][2])
	We[13] = 7;	// cost(A[5][3]) - x7
	We[14] = 5;	// cost(A[5][4]) - x8


	try {
		GRBEnv env = GRBEnv();

		GRBModel model = GRBModel(env);
		
		//model.getEnv().set(GRB_IntParam_OutputFlag, 0);

		// Create variables
		//GRBVar x0 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x0");
		//GRBVar x1 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x1");
		//GRBVar x2 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x2");
		//GRBVar x3 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x3");
		//GRBVar x4 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x4");
		//GRBVar x5 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x5");
		//GRBVar x6 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x6");
		//GRBVar x7 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x7");
		//GRBVar x8 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x8");
		
		double lb[9] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
		double ub[9] = {1,1,1,1,1,1,1,1,1};
		double obj[9] = {0,0,0,0,0,0,0,0,0};
		char type[9] = {GRB_BINARY,GRB_BINARY,GRB_BINARY,GRB_BINARY,GRB_BINARY,GRB_BINARY,GRB_BINARY,GRB_BINARY,GRB_BINARY};
		const string names[9] = {"x0","x1","x2","x3","x4","x5","x6","x7","x8"};
		
		GRBVar* X = model.addVars(lb,ub,obj,type,names,9);

		// Integrate new variables
		model.update();

		// Set objective: minimize w0*x0 + w1*x1 + ... + wn*xn
		//model.setObjective(We[0]*x0 + We[1]*x1 + We[2]*x2 + We[3]*x3 + We[4]*x4 + We[5]*x5 + We[9]*x6 + We[13]*x7 + We[14]*x8, GRB_MINIMIZE);
		model.setObjective(We[0]*X[0] + We[1]*X[1] + We[2]*X[2] + We[3]*X[3] + We[4]*X[4] + We[5]*X[5] + We[9]*X[6] + We[13]*X[7] + We[14]*X[8], GRB_MINIMIZE);

		// Add constraint: x0 + x1 + ... + xn = n - 1
		//model.addConstr(x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 == n - 1, "c0");
		//model.addConstr(X[0] + X[1] + X[2] + X[3] + X[4] + X[5] + X[6] + X[7] + X[8] == n - 1, "c0");
		
		// Make expression as: x0 + x1 + ... + xn
		GRBLinExpr expr0 = X[0];
		for(int i = 1; i < 9; i++) expr0+=(1, X[i]);
		
		// Add constraint: x0 + x1 + ... + xn = n - 1
		model.addConstr(expr0,GRB_EQUAL,n-1,"c0");
		
		cout << "expr:" << expr0 << endl;
		
		// Add constraint: x + y >= 1
		//model.addConstr(x0 + x1 + x3 >= 1, "c1");
		//model.addConstr(x1 + x2 >= 1, "c2");
		//model.addConstr(x0 + x1 >= 1, "c3");
		//model.addConstr(x3 + x4 + x5 + x6 >= 1, "c4");
		//model.addConstr(x2 + x4 >= 1, "c5");
		//model.addConstr(x6 + x7 >= 1, "c6");
		//model.addConstr(x7 + x8 >= 1, "c7");
		//model.addConstr(x6 + x8 >= 1, "c8");

		// Optimize model
		model.optimize();

		cout << X[0].get(GRB_StringAttr_VarName) << " = " << X[0].get(GRB_DoubleAttr_X) << endl;
		//cout << x1.get(GRB_StringAttr_VarName) << " = " << x1.get(GRB_DoubleAttr_X) << endl;
		//cout << x2.get(GRB_StringAttr_VarName) << " = " << x2.get(GRB_DoubleAttr_X) << endl;
		//cout << x3.get(GRB_StringAttr_VarName) << " = " << x3.get(GRB_DoubleAttr_X) << endl;
		//cout << x4.get(GRB_StringAttr_VarName) << " = " << x4.get(GRB_DoubleAttr_X) << endl;
		//cout << x5.get(GRB_StringAttr_VarName) << " = " << x5.get(GRB_DoubleAttr_X) << endl;
		//cout << x6.get(GRB_StringAttr_VarName) << " = " << x6.get(GRB_DoubleAttr_X) << endl;
		//cout << x7.get(GRB_StringAttr_VarName) << " = " << x7.get(GRB_DoubleAttr_X) << endl;
		//cout << x8.get(GRB_StringAttr_VarName) << " = " << x8.get(GRB_DoubleAttr_X) << endl;
		

		cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	} catch(GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	} catch(...) {
		cout << "Exception during optimization" << endl;
	}

	return 0;
}

