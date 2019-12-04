cmake_minimum_required(VERSION 3.15)
project(NoGu)

include_directories("include")

set(CMAKE_CXX_STANDARD 11)

add_executable(NoGu
        main.cpp
        include/nogu.h)

add_executable(gualloctor_test test/gualloctor_test.cpp test/test.h)

add_executable(mector_test test/mector_test.cpp test/test.h)

add_executable(gustring_test test/gustring_test.cpp test/test.h base/gustring.h base/gustring.cpp)