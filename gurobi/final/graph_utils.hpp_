/**********************************************************************
*	Tools for access, manage and process graphs by its adjacency matrix
*	Written by Sidney RDC, 2014.
***********************************************************************/

#ifndef GRAPHUTILS_H
#define GRAPHUTILS_H

#include <iostream>
#include <armadillo>

#define MIN_ERR 1e-3

// Logic or between two adjacency matrices
arma::mat logic_or(arma::mat x, arma::mat y);

// Reachability for an adjacency matrix A
arma::mat reach_matrix(arma::mat A);

// Reduce the adjacency matrix to a reachable neighbors adjacency matrix
std::vector<arma::mat> min_mat(int id, arma::mat A);

// Verify if a node is critical
bool is_critical(arma::mat A, unsigned int node);

// Find all critical nodes in the graph
arma::uvec critical_nodes(arma::mat A);

#endif
