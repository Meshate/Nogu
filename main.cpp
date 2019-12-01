#include <iostream>
#include "base/threadpool.h"
#include "base/mector.h"
#include <vector>
#include <memory>

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

void mec_test() {
    nogu::mector<int> a{22, 421, 222};
    cout << a << endl;
    if (!a.empty())a.push_back(213);
    a.push_back(43232);
    int t = 32;
    a.push_back(t);
    cout << a << endl;

    nogu::mector<int> c(a);
    a.push_back(3432234);
    c.emplace_back(1);
    cout << a.front() << endl;
    cout << c.back() << endl;
}

void mec_bench() {
    const int t1 = 10000;
    const int t2 = 10000;
    {
        vector<int> t;
        clock_t cl;
        cl = clock();
        for (int i = 0; i < t1; ++i) {
            for (int j = 0; j < t2; ++j) {
                t.push_back(i + j);
            }
        }
        for (int i = 0; i < t1; ++i) {
            for (int j = 0; j < t2; ++j) {
                t.pop_back();
            }
        }
        if (t.empty())cout << "vector OK\n";
        cout << "time: " << clock() - cl << endl;
    }

    {
        nogu::mector<int> t;
        clock_t cl;
        cl = clock();
        for (int i = 0; i < t1; ++i) {
            for (int j = 0; j < t2; ++j) {
                t.push_back(i + j);
            }
        }
        for (int i = 0; i < t1; ++i) {
            for (int j = 0; j < t2; ++j) {
                t.pop_back();
            }
        }
        if (t.empty())cout << "mector OK\n";
        cout << "time: " << clock() - cl << endl;
    }
}

int main() {
//    thread_test();

//    mec_test();

//    bench1();
//    bench2();

    return 0;
}