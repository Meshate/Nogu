#include <iostream>
#include "base/threadpool.h"
#include "base/mector.h"
#include <vector>

using namespace std;

void thread_test() {
    int num = 0;
    {
        nogu::threadpool pool(4);

        pool.add_task([](int *p){
                ++(*p);
            }, &num);
    }
    cout<<num<<endl;
}

int main() {
    thread_test();


    return 0;
}