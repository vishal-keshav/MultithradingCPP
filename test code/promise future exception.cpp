#include <iostream>
#include <thread>
#include <string>
#include <future>
#include <chrono>

using namespace std;


void fun(promise<string> &prms){
//void fun(promise<string> &&prms){
	try{
		string str("Data of thread");
		//throw(exception("man made exception"));
		//setting promise value for passing in another thread
		prms.set_value(str);
		this_thread::sleep_for(chrono::seconds(1));
	}
	//If exception occure, catch them all
	catch(...){
		//set_exception is pre build function
		prms.set_exception(current_exception());
	}
}

int main(){

	promise<string> prms;
	future<string> ftr = prms.get_future();
	//promise is moved as a reference as parameter to function
	thread t(&fun,ref(prms));
	//thread t(&fun,move(prms));
	
	try{
		//Future get is a blocking call
		string str = ftr.get();
		cout << str << endl;
	}
	catch(exception &e){
		//Exception funtion is provided in C++11 standard
		cout << e.what() << endl;
	}
	t.join();
	return 0;
}