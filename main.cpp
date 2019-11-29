#include <iostream>
#include "base/threadpool.h"
#include "base/mector.h"
#include <vector>

using namespace std;

void thread_test(){
    int num = 0;
    {
        nogu::threadpool pool(8);

        for (int i = 0; i < 100; ++i) {
            pool.add_task([&] { ++num; });
        }
    }
    cout << num << endl;
}

void mec_test(){
    nogu::mector<int> a{22,421,222};
    cout<<a<<endl;
    if(!a.empty())a.push_back(213);
    a.push_back(43232);
    int t = 32;
    a.push_back(t);
    cout<<a<<endl;

    nogu::mector<int>c(a);
    a.push_back(3432234);
    cout<<a.front()<<endl;
    cout<<c.back()<<endl;
}

int main() {
//    thread_test();
//
    mec_test();


    return 0;
}