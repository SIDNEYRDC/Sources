/***********************************************************************
*	k - Minimal Spanning Tree Algorithm (k-MST) with Integer Programming
*	solved by Gorubi©. Written by Sidney RDC, 2014.
***********************************************************************/

#ifndef KOPT_H
#define KOPT_H

#include <iostream>
#include <armadillo>
#include "/opt/gurobi563/linux64/include/gurobi_c++.h"

// Typedef with edge data
typedef struct edge {
	unsigned int i;	// i vertice
	unsigned int j;	// j vertice
	double w;		// weight of edge
} edge;

// Enumerate all connected subgraphs on a graph
void subgraph(arma::umat &S, std::vector<int> sg, arma::mat A, arma::uvec N, unsigned int depth, int &id_s, int id_sg);

// Overload of subgraph generation
void subgraph(arma::umat &S, arma::mat A, unsigned int depth);

// Generate a Minimal Spanning Tree for a weighted adjacency matrix A
arma::mat mst_optimization(arma::mat A);

// Generate a bi-connected Minimal Spanning Tree for a weighted adjacency matrix A
arma::mat k2_optimization(arma::mat A);

#endif
