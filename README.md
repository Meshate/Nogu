# Nogu
一个闲来无事把之前写的东西整理并重写了一下合起来的垃圾轮子库
## threadpool
一个简陋的高性能(应该)线程池
### example
```c++
#include "base/threadpool"
#include <iostream>

using namespace std;

int main(){
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
    return 0;
}
```
## mector
一个高性能可变长数组
### example
```c++
#include <iostream>
#include "base/mector.h"
using namespace std;

int main(){ //just use like vector
    nogu::mector<int> a{22, 421, 222};
    if (!a.empty())a.push_back(213);
    a.push_back(43232);
    int t = 32;
    a.push_back(t);

    nogu::mector<int> c(a);
    a.push_back(3432234);
    c.emplace_back(1);
    cout << a.front() << endl;
    cout << c.back() << endl;
    
    return 0;
}
```
## gualloctor
线程安全的内存池
### example
```c++
#include "base/gulloctor"

#ifndef THREADS_SAFE
#define THREADS_SAFE //启用线程安全
#endif

int main(){
    nogu::gulloctor<int> alloc; //建立gulloctor对象，内存分配单元类型为int
    auto a = alloc.allocate(1); //申请内存
    alloc.construct(a,123); //在申请的内存上构建对象
    alloc.destroy(a); //析构对象
    alloc.deallocate(1); //释放已申请的内存
    return 0;
}
```
## gustring(TODO)
字符串库
## gutrans(TODO)
高性能数字转字符串库(规划中，先学会数学)
## more
