#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

// Combine Cn,d distint
void combine(vector<unsigned int> S, unsigned int id, unsigned int index, unsigned int depth, unsigned int n) {
	for(unsigned int i = id; i < n - (depth - 1); i++) {
		S[index] = i;

		if(depth > 1) combine(S,i+1,index+1,depth-1,n);
		else {
			for(unsigned int k = 0; k < S.size(); k++) {
				cout << S[k] << " ";
			}
			cout << endl;
		}
	}
}

// Overload in combine
void combine(vector<unsigned int> S, unsigned int depth, unsigned int n) {
	combine(S,0,0,depth,n);
}

void subgraph(vector<int> S, mat A, uvec N, int elem, int depth, int index);
void subgraph(vector<int> S, mat A, int depth);

int main()
{
	unsigned int n = 20;

	for(unsigned int size = 3; size <= n / 2; size++) {
		vector<unsigned int> S(size);
		combine(S,size,n);
	}
    //int n = 6;
    //int moedas[8] = {1, 2, 5, 10, 20, 50, 100, 200};
    //int l0, l1, l2, l3;
    //int total = 0;

    //for (l0=0; l0<n-3; l0++)
    //{
        //for (l1=l0+1; l1<n-2; l1++)
        //{
            //for (l2=l1+1; l2<n-1;l2++)
            //{
                //for (l3=l2+1; l3<n;l3++)
                //{
                    //cout << l0 << " " << l1 << " " << l2 << " " << l3 << endl;
                    //total++;
                //}
            //}
        //}
    //}
    ////cout << total << endl;


	//int n = 6;
	//// Weighted adjacency matrix
	//mat A = zeros<mat>(n,n);

	//A
	//<< 0 << 3 << 5 << 0 << 0 << 7 << endr
	//<< 3 << 0 << 4 << 0 << 0 << 0 << endr
	//<< 5 << 4 << 0 << 6 << 0 << 0 << endr
	//<< 0 << 0 << 6 << 0 << 1 << 2 << endr
	//<< 0 << 0 << 0 << 1 << 0 << 3 << endr
	//<< 7 << 0 << 0 << 2 << 3 << 0 << endr;

	//vector<int> S(3);

	////vector< vector<int> > S(1, vector<int>(3));

	//subgraph(S,A,3);
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

//void subgraph(vector<int> S, mat A, int depth) {
	//unsigned int n = A.n_rows;
	//unsigned int index = 0;
	//for(unsigned int i = 0; i < n; i++) {
		//S[index] = i;
		//uvec N = find(A.col(i));
		//subgraph(S,A,N,i,depth-1,index+1);
	//}
//}

//void subgraph(vector<int> S, mat A, uvec N, int elem, int depth, int index) {
	//for(int i = 0; i < N.n_elem; i++) {
		////if(N(i) > elem) {
		//if(find(S.begin(), S.end(), N(i)) == S.end()) {
			//S[index] = N(i);
			//uvec N_j = find(A.col(N(i)));
			//if(depth > 1) subgraph(S,A,N_j,N(i),depth-1,index+1);
			//else {
				//for(unsigned int k = 0; k < S.size(); k++) {
					//cout << S[k] << " ";
				//}
				//cout << endl;
			//}
		//}
	//}
//}

