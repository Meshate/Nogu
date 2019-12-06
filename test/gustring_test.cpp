//
// Created by meshate on 2019/12/4.
//

#include "../base/gustring.h"
#include "test.h"

int main(){
    def_test(1000000);

    nogu::gustring ss = "aaa";
    std::string as = "aaa";

    nogu::gustring gs;
    std::string ds;

    assert(ss.match(".*"));
    assert(ss+"abc" == "aaaabc");
    assert(ss.slice(2) == "a");
    assert(ss>"a");

    def_case(nogu::gustring("abcde"));
    def_case(std::string("abcde"));

    std::cout<<"========= copy =========\n";
    def_case(gs = nogu::gustring("abcde"));
    def_case(ds = std::string("abcde"));
    def_case(gs = "");
    def_case(ds = "");
    def_case(gs = ss);
    def_case(ds = as);

    std::cout<<"========= append =========\n";
    def_case(gs.append('a'));
    def_case(ds.append("a"));
    def_case(gs.append("aasdas"));
    def_case(ds.append("aasdas"));
    def_case(gs.append(ss));
    def_case(ds.append(as));

    return 0;
}

