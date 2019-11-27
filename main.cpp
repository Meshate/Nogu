#include <iostream>
#include <atomic>
#include <chrono>
#include <ctime>
#include "threadpool.h"

using namespace std;

static atomic<int> num = 0;



int main() {
    {
        nogu::threadpool pool(4);
//    vector<long> test{12321532,6533222662,5645445,2235253255,56346534,3456376,653636,346654,234,2};
//    for(auto &i:test){
//        auto use = std::bind(check_prime,i);
//        pool.add_task(use);
//    }
        for (int i = 0; i < 100; ++i) {
            pool.add_task([] { ++num; });
        }
    }

    cout<<num<<endl;

    return 0;
}