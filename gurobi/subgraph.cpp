#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;


// Enumerate all connected subgraphs on a graph
void subgraph(umat &S, vector<int> sg, mat A, uvec N, unsigned int depth, int &id_s, int id_sg) {
	for(unsigned int i = 0; i < N.n_elem; i++) {
		if(find(sg.begin(), sg.end(), N(i)) == sg.end()) {
			sg[id_sg] = N(i);
			uvec N_j = find(A.col(N(i)));
			if(depth > 1) subgraph(S,sg,A,N_j,depth-1,id_s,id_sg+1);
			else {
				id_s++;
				S.resize(id_s+1,S.n_cols);

				for(unsigned int k = 0; k < S.n_cols; k++) {
					S(id_s,k) = sg[k];
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

	for(unsigned int i = 0; i < n; i++) {
		sg[id_sg] = i;
		uvec N = find(A.col(i));
		subgraph(S,sg,A,N,depth-1,id_s,id_sg+1);
	}
}

int main()
{
	int n = 6;
	// Weighted adjacency matrix
	mat A = zeros<mat>(n,n);

	A
	<< 0 << 3 << 5 << 0 << 0 << 0 << endr
	<< 3 << 0 << 4 << 0 << 0 << 0 << endr
	<< 5 << 4 << 0 << 6 << 0 << 0 << endr
	<< 0 << 0 << 6 << 0 << 1 << 2 << endr
	<< 0 << 0 << 0 << 1 << 0 << 3 << endr
	<< 0 << 0 << 0 << 2 << 3 << 0 << endr;

	umat S = zeros<umat>(1,3);
	subgraph(S,A,3);

	for(unsigned int i = 0; i < S.n_rows; i++) {
		cout << S(i,span()) << endl;
	}

	//vector< vector<int> > S(1, vector<int>(3));
	//for(int size = 3; size <= n / 2; size++) {
		//for(int i = 0; i < n; i++) {
			//uvec N = find(A.col(i));
			//subgraph(S,A,N,i,3,0);
			//uvec N = find(A.col(i));
			//for(int n = 0; n < N.n_elem; n++) {
				//if(N(n) > i) {
					////cout << i << " " << N(n) << endl;
					//uvec N_j = find(A.col(N(n)));
					//for(int nn = 0; nn < N_j.n_elem; nn++) {
						//if(N_j(nn) > N(n)) {
							//cout << i << " " << N(n) << " " << N_j(nn) << endl;
						//}
					//}
				//}
			//}
		//}
	//}

    return 0;
}


