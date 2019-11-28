#include <iostream>
#include "base/threadpool.h"
#include "base/mector.h"

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
    nogu::mector<int> a;
    if(a.empty())a.push_back(213);
    a.push_back(43232);
    int t = 32;
    a.push_back(t);
    cout<<a<<endl;
    cout<<t<<endl;
    a.pop();
    cout<<a<<endl;
}

int main() {
    thread_test();

    mec_test();

    return 0;
}