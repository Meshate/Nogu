//
// Created by meshate on 2019/12/2.
//

#ifndef NOGU_GUALLOCTOR_TEST_H
#define NOGU_GUALLOCTOR_TEST_H

#include "gualloctor.h"

#include <memory>
#include <iostream>

template<typename T, typename Alloc = std::allocator<T> >
class StackAlloc {
public:

    StackAlloc() { _head = 0; }

    ~StackAlloc() { clear(); }

    bool empty() { return (_head == 0); }

    void clear() {
        _Node *curr = _head;
        while (curr != 0) {
            _Node *tmp = curr->prev;
            _alloc.destroy(curr);
            _alloc.deallocate(curr, 1);
            curr = tmp;
        }
        _head = 0;
    }

    void push(T element) {
        _Node *newNode = _alloc.allocate(1);
        _alloc.construct(newNode, _Node());

        newNode->data = element;
        newNode->prev = _head;
        _head = newNode;
    }

    T pop() {
        T result = _head->data;
        _Node *tmp = _head->prev;
        _alloc.destroy(_head);
        _alloc.deallocate(_head, 1);
        _head = tmp;
        return result;
    }

    T top() { return (_head->data); }

private:
    struct _Node {
        T data;
        _Node *prev;
    };
    typedef typename Alloc::template rebind<_Node>::other allocator;

    allocator _alloc;
    _Node *_head;
};

const int ELEMS = 1000000;
const int REPS = 100;

void gulloctor_test() {
    {
        clock_t start = clock();
        StackAlloc<int> a;
        for (int j = 0; j < REPS; j++) {
            assert(a.empty());
            for (int i = 0; i < ELEMS; i++)
                a.push(i);
            for (int i = 0; i < ELEMS; i++)
                a.pop();
        }
        std::cout << "Default Allocator Time: ";
        std::cout << (((double) clock() - start) / CLOCKS_PER_SEC) << std::endl;
    }
    {
        clock_t start = clock();
        StackAlloc<int, nogu::gulloctor<int>> a;
        for (int j = 0; j < REPS; j++) {
            assert(a.empty());
            for (int i = 0; i < ELEMS; i++) {
                a.push(i);
            }
            for (int i = 0; i < ELEMS; i++) {
                a.pop();
            }
        }
#ifdef THREADS_SAFE
        std::cout << "thread safe ";
#endif
        std::cout << "gullocator Time: ";
        std::cout << (((double) clock() - start) / CLOCKS_PER_SEC) << std::endl;
    }
}

#endif //NOGU_GUALLOCTOR_TEST_H