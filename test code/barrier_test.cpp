/*This is a simple test program that tests funtionality of barrier
class object. Threads are created doing specific task within iteration
Each thread process upon completion of an iteration waits for every
other to finish. Whecn each thread reaches barrier point, next
iteration starts*/ 

#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <mutex>
#include <condition_variable>


using namespace std;

//Barrier Class
class Barrier
{
private:
    mutex _mutex;
    condition_variable _cv;
    size_t _count;
    int _Gcount;
public:
    Barrier(size_t count){
    	_count = count; 
    	_Gcount = count;
    }
    void Wait()
    {
        unique_lock<mutex> lock{_mutex};
        if (--_count == 0) {
            _cv.notify_all();
        } else {
            _cv.wait(lock, [this] { return _count == 0; });
        }
    }
    void reset(int t_id){
    	if(t_id==0){
    		_count = _Gcount;
    	}
    }
};

void task(Barrier &b,int t_id){
	for(int i=0;i<2;i++){	
		printf("Iteration %d from thread %d\n",i,t_id);
		b.Wait();
	}
}

int main(){
	//Creating a barrier in main thread (Later shared by threads)
	Barrier b(3); //Assuming three threads are to be created
	//Creating thread vector to retain the thread information
	vector<thread> threads;
    for(int i=0;i<3;i++){
        //Pushing thread while creating it
        threads.push_back(thread(task,ref(b),i));
    }
    //Waiting for above created threads
    for_each(threads.begin(),threads.end(),[](thread &t){t.join();});

	cout << "Each threads finished" << endl;
	return 0;
}