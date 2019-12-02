#include <iostream>
#include "base/threadpool.h"
#include "base/mector.h"
#include <vector>
#include "base/gualloctor_test.h"

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
    const int t1 = 10000;
    const int t2 = 10000;
    {
        vector<int> t;
        clock_t start;
        start = clock();
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
        cout << "time: " << (((double) clock() - start) / CLOCKS_PER_SEC) << endl;
    }

    {
        nogu::mector<int> t;
        clock_t start;
        start = clock();
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
        cout << "time: " << (((double) clock() - start) / CLOCKS_PER_SEC) << endl;
    }
}

int main() {
    {
        thread_test();
    }
    {
        mec_test();
    }
    {
        gulloctor_test();
    }

    return 0;
}