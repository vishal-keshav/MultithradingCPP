#include <iostream>
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <math.h>

using namespace std;

//Shared memory between threads
vector<int> data;
vector<int> sum;

//Barrier class to synchronize between different threads
class Barrier
{
private:
    std::mutex _mutex;
    std::condition_variable _cv;
    std::size_t _count;
public:
    explicit Barrier(std::size_t count) : _count{count} { }
    void Wait()
    {
        std::unique_lock<std::mutex> lock{_mutex};
        if (--_count == 0) {
            _cv.notify_all();
        } else {
            _cv.wait(lock, [this] { return _count == 0; });
        }
    }
};

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

void thread_sum(int t_id,int p){
    //Thread will figure it out on which part it have to operate
    int n = size(data);
    int start = t_id*(n/p);
    int end = (n>(t_id+1)*(n/p))?(t_id+1)*(n/p):n;
    for(int i=start+1;i<end;i++){
        data[i] = data[i-1]+data[i];
    }
    sum[t_id] = data[end-1];
}

void thread_collection(int myid,int p,Barrier &barrier){
    int otherid;
    int temp;
    for(i=1;i<(int)log2(p);i++){
        if((myid+1)%pow(2,i) == 0){
            otherid = myid - pow(2,(i-1));
            temp = sum[otherid];
            sum[myid] = sum[myid] + temp;
        }
        barrier.Wait();
    }
}

void thread_distribute(int myid,int p,Barrier &barrier){
    int otherid;
    int temp;
    for(i=(int)log2(p)-1;i>0;i--){
        if((myid+1)%2 == 0){
            otherid = myid - 2^(i-1)
            temp = sum[otherid];
            sum[otherid] = sum[myid];
            sum[myid] = mysum[myid] + temp;
        }
        barrier.Wait();
    }
}

void thread_sum_final(int t_id,int p){
    //Thread will figure it out on which part it have to operate
    int n = size(data);
    int start = t_id*(n/p);
    int end = (n>(t_id+1)*(n/p))?(t_id+1)*(n/p):n;
    for(int i=start;i<end;i++){
        data[i] = sum[t_id]+data[i];
    }
}

int main(){
	int element_size = 20;
    data = create_data(element_size);
    show(data);
    //Number of threads to create
    int p = 2;
    sum = vector<int>(p);

    //Creation and distribution of data to threads
    vector<thread> threads;
    for(int i=1;i<p;i++){
        //Pushing thread while creating it
        threads.push_back(thread(thread_sum,i,p));
    }
    for_each(threads.begin(),threads.end(),[](thread &t){t.join();});

    //Collection phase
    Barrier b = new Barrier(p);
    threads.clear();
    for(int i=1;i<p;i++){
        //Pushing thread while creating it
        threads.push_back(thread(thread_collection,i,p,ref(b)));
    }
    //Do own process
    int otherid;
    int myid = 0;
    int temp;
    for(i=0;i<(int)log2(p);i++){
        if((myid+1)%pow(2,i) == 0){
            otherid = myid - pow(2,(i-1));
            temp = sum[otherid];
            sum[myid] = sum[myid] + temp;
        }
        b.Wait();
    }
    for_each(threads.begin(),threads.end(),[](thread &t){t.join();});

    //Last sum it zeroed
    sum[size(sum)-1] = 0;

    //Distribution phase
    Barrier b2 = new Barrier(p);
    threads.clear();
    for(int i=1;i<p;i++){
        //Pushing thread while creating it
        threads.push_back(thread(thread_distribution,i,p,ref(b2)));
    }
    //Do own process
    for(i=(int)log2(p)-1;i>0;i--){
        if((myid+1)%2 == 0){
            otherid = myid - 2^(i-1)
            temp = sum[otherid];
            sum[otherid] = sum[myid];
            sum[myid] = mysum[myid] + temp;
        }
        b2.Wait();
    }
    for_each(threads.begin(),threads.end(),[](thread &t){t.join();});

    //Adding sum to all element
    threads.clear();
    for(int i=1;i<p;i++){
        //Pushing thread while creating it
        threads.push_back(thread(thread_sum_final,i,p));
    }
    for_each(threads.begin(),threads.end(),[](thread &t){t.join();});

    show(data);

	return 0;
}