/**********************************************************************
*	Tools for default C\C++ Programming
*	Written by Sidney RDC, 2014.
***********************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
#include <armadillo>
#include <QuadProg++.hh>
#include <time.h>
#include "defines.hpp"

// Return the maximum between two floats
float max_float(float x, float y);

// Show a matrix of QuadProg type
void show_matrix(const QuadProgPP::Matrix<double> mat);

// Show a vector of QuadProg type
void show_vector(const QuadProgPP::Vector<double> vec);

// Return the distance between two vectors containing x and y coordinates
float dist(arma::rowvec a, arma::rowvec b);

// Compute the time between two times
double calc_time(clock_t t0, clock_t t1);

// Fill a std::vector with arma::rowvec elements
void fill_vec(std::vector<int> &vec, arma::urowvec elems);

// Find a value in a vector and return its position
bool find_value(int value, std::vector<int> vec, unsigned int &pose);

// Find a value in a vector
bool find_value(int value, std::vector<int> vec);

// Search a character in a string and return this position
bool find_char(std::string input, char search, unsigned int &pos);

// Search a character in a string
bool find_char(std::string input, char search);

// Verify if a string input contains a substring search and return the start pose and end pose of this
bool contains(std::string input, std::string search, unsigned int &init_pose, unsigned int &end_pose);

// Verify if a string input contains a substring search
bool contains(std::string input, std::string search);

// Verify if a unsigned int array contain a element
bool contains(unsigned int elem, arma::uvec array);

// Extract all float numbers on a string
std::vector<float> get_float(std::string input);

// Allocate 2D arrays
template <class T> T **create2D(int rows, int cols) {
    T **array2D = new T *[rows];

    for(unsigned int i = 0; i < rows; i++) {
		array2D[i] = new T [cols];
	}

	for(unsigned int i = 0; i < rows; i++) {
		for(unsigned int j = 0; j < cols; j++) {
			array2D[i][j] = 0;
		}
	}

	return array2D;
}

#endif
