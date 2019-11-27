#include <iostream>
#include "threadpool.h"

using namespace std;

int main() {
    int num = 0;
    {
        nogu::threadpool pool(8);

        for (int i = 0; i < 100; ++i) {
            pool.add_task([&] { ++num; });
        }
    }

    cout << num << endl;

    return 0;
}