#include <iostream>
#include <cstdlib>
#include <complex>
#include <cmath>
#include <vector>


using namespace std;

double const PI = 4 * std::atan(1);

int main(){
	vector<complex<double> > v;
	for(int i=0;i<4;i++){
		complex<double> cnum(cos((2*PI*i)/4),sin((2*PI*i)/4));
		v.push_back(cnum);
		//cout << cnum << endl;
	}
	for(int i=0;i<v.size();i++){
		cout << v[i] << endl;
	}
	return 0;
}