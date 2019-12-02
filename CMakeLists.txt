cmake_minimum_required(VERSION 3.15)
project(NoGu)

include_directories("/usr/local/include")

set(CMAKE_CXX_STANDARD 17)

add_executable(NoGu
        main.cpp
        base/threadpool.h
        base/mector.h
        base/gualloctor.h
        base/gualloctor_test.h
#        base/gustring.h
#        base/gustring.cpp
        )