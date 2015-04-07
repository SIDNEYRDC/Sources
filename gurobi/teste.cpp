#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

int main() {
	vector<int> v(1);
    
    for(int i = 0; i < 1; i++) v[i] = i;
    
    for(int i = 0; i < v.size(); i++) {
        cout << "->" << v[i] << endl;
        v.erase(v.begin()+i);
        cout << "rm->" << v[i] << endl;
        i--;
    }
}
