#include <thread>
#include <future>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int simplefunc(int a)
{ 
    return a;
}



vector<int> fib(int n){
    vector<int> v;
    if(n<=1){
        v.push_back(n);
        return v;
    }
    vector<int> fib2(1);
    //int fib1;
    auto fib1 = async([=]{return fib(n-1);});
    //thread t([=, &fib1]{fib1 = fib(n-1);});
    fib2 = fib(n-2);
    //t.join();
    //return fib1 + fib2;
    v.push_back(fib1.get()[0] + fib2[0]);
    return v;
}

int main()
{
    vector<int> simple = fib(4);
    cout << simple[0] << endl;
    return 0;
}