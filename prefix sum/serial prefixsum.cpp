#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

//Function to create random int data
vector<int> create_data(int n){
	vector<int> v(n);
	for(vector<int>::iterator it = v.begin() ; it!= v.end(); ++it){
		*it = rand()%100;
	}
	return v;
}
//Funtion to print the data on console
void show(vector<int> &v){
	int n = v.size();
	for(vector<int>::iterator it = v.begin() ; it!= v.end(); ++it){
		cout << *it << " ";
	}
	cout << endl;
}
//Function for doing prefix sum
void prefix_sum(vector<int> &v){
	int n = v.size();
	for(vector<int>::iterator it = v.begin()+1 ; it!= v.end(); ++it){
		*it = *it + *(it-1);
	}
}
//Main Function
int main(){
	int element_size = 20;
	vector<int> data = create_data(element_size);
	show(data);
	prefix_sum(data);
	show(data);
	return 0;
}