#include <iostream>
#include <thread>

using namespace std;

//Function executed by threads created in main thread
void prime(int start,int end){
	//This is not an optimal way of checking prime
	//Checks for prime number in the input range
	if(start>end || start<2){
		cout << this_thread::get_id() << ": Incorrect output" << endl;
	}
	else{
		for(int i=start;i<=end;i++){
			bool flag = false;
			for(int j=2;j<i;j++){
				if(i%j==0){
					flag = true;
					break;
				}
			}
			if(!flag){
				cout << this_thread::get_id() << ": "<< i << " is prime" << endl;
			}
		}
	}
}

int main(){
	//Creating two threads
	thread t1(prime,820000,820500);
	thread t2(prime,820000,820800);
	//Waiting for above created threads
	t1.join();
	t2.join();

	cout << "Each threads finished" << endl;
	return 0;
}