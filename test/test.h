//
// Created by meshate on 2019/12/3.
//

#ifndef NOGU_TEST_H
#define NOGU_TEST_H

#include <iostream>

#define def_test(n) \
    int N = n; \
    clock_t start

#define def_case(func) \
    start = clock(); \
    for (int i = 0; i < N; ++i) { \
        func; \
    } \
    std::cout << #func << ": " << (((double) clock() - start) / CLOCKS_PER_SEC)<<std::endl

#endif //NOGU_TEST_H
