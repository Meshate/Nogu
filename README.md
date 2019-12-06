# Nogu
一个闲来无事把之前写的东西整理并重写了一下合起来的轮子库
## threadpool
一个简陋的高性能(应该)线程池
### example
```c++
#include "nogu.h"
#include <iostream>

using namespace std;

int main(){
    int num = 0;
        {
            nogu::threadpool pool(4);
    
            pool.add_task([](int *p){
                    ++(*p);
                }, &num);//若参数为引用则无法改变参数的值
        }
        cout<<num<<endl;
    return 0;
}
```
## mector
一个高性能可变长数组
### example
```c++
#include <iostream>
#include "nogu.h"
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
#include "nogu.h"

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
## gustring
高性能字符串库，支持`.`和`*`的正则表达式匹配和slice切片。
### example
```c++
#include "nogu.h"
#include <iostream>

using namespace std;

int main(){
    nogu::gustring s("aaa");
    s.match("a*");
    s.append("dd");
    s.match("a*.*");
    
    return 0;
}
```
## gutrans(TODO)
高性能数字转字符串库(规划中，先学会数学)
## more
