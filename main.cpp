#include <iostream>
#include "nogu.h"

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

    nogu::gustring s("abcd");

    return 0;
}