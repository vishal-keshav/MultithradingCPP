#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

vector<int> coeff(int len){
	vector<int> ret(len,0);
	for(int i=0;i<len;i++){
		ret[i] = rand()%10;
	}
	return ret;
}

void show(vector<int> v){
	for(int i=0;i<v.size();i++){
		cout << v[i] << " ";
	}
	cout << endl;
}

vector<int> sum(vector<int> a,vector<int> b){
	vector<int> ret(a.size(),0);
	if(a.size()!=b.size()){
		cout << "Incompatible" << endl;
	}
	else{
		for(int i=0;i<a.size();i++){
			ret[i] = a[i]+b[i];
		}
	}
	return ret;
}

vector<int> prod(vector<int> a,vector<int> b){
	vector<int> ret(a.size()+b.size()-1,0);
	a.resize(ret.size());
	b.resize(ret.size());
	for(int i=0;i<ret.size();i++){
		ret[i]=0;
		for(int j=0;j<=i;j++){
			ret[i] = ret[i] + a[j]*b[i-j];
		}
	}
	return ret;
}

int main(){
	int degree = 2;
	vector<int> a = coeff(degree);
	vector<int> b = coeff(degree);
	vector<int> ans_sum = sum(a,b);
	vector<int> ans_prod = prod(a,b);
	show(a);
	show(b);
	show(ans_sum);
	show(ans_prod);

	return 0;
}