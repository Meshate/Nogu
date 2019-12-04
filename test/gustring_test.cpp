//
// Created by meshate on 2019/12/4.
//

#include "../base/gustring.h"
#include "test.h"

int main(){
    def_test(100000);

    nogu::gustring ss = "aaa";

    assert(ss.match(".*"));

    return 0;
}

