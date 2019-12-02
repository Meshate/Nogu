#include <iostream>
#include "base/threadpool.h"
#include "base/mector.h"
#include <vector>

using namespace std;

void thread_test() {
    mutex rt;
    int num = 0;
    {
        nogu::threadpool pool(8);

        for (int i = 0; i < 100; ++i) {
            pool.add_task([&] {
                lock_guard<mutex> lk(rt);
                ++num;
                cout << num << endl;
            });
        }
    }
}

int main() {
    nogu::mector<int> a{1213,213,4532};
    a.insert(a.begin(),111);
    cout<<a<<endl;

    return 0;
}