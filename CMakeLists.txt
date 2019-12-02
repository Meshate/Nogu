cmake_minimum_required(VERSION 3.15)
project(NoGu)

include_directories("/usr/local/include")
include_directories("./")

set(CMAKE_CXX_STANDARD 17)

add_executable(NoGu
        main.cpp
        base/threadpool.h
        base/mector.h
        base/gualloctor.h
        base/gustring.h
        base/gustring.cpp
        )

add_executable(gualloctor_test test/gualloctor_test.cpp test/test.h)

add_executable(mector_test test/mector_test.cpp test/test.h)