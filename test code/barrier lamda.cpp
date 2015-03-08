#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>

using namespace std;

int main(){

	//Storing the information about all created threads
	vector<thread> threads;
	for(int i=0;i<10;i++){
		//Pushing thread while creating it
		//Lambda: [Capture variable as private](arguments){body of function}
		threads.push_back(thread([i](){cout << i << endl;}));
	}

	cout << "Main function" << endl;
	//for_each implementation from algorithm
	//thread from threads is passed as reference
	for_each(threads.begin(),threads.end(),[](thread &t){t.join();});

	return 0;
}