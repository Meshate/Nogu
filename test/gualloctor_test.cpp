//
// Created by meshate on 2019/12/2.
//

#include "base/gualloctor.h"
#include "test.h"
#include <memory>

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

int main() {
    def_test(1000000);

    StackAlloc<int> ta;
    StackAlloc<int, nogu::gulloctor<int>> tb;
    std::cout<<"========= push =========\n";
    def_case(ta.push(0));
    def_case(tb.push(0));
    def_case(ta.push(0x7fffffff));
    def_case(tb.push(0x7fffffff));
    def_case(ta.push(0xffffffff));
    def_case(tb.push(0xffffffff));

    std::cout<<"========= pop =========\n";
    def_case(ta.pop());
    def_case(tb.pop());

    std::cout<<"========= clear =========\n";
    def_case(ta.clear());
    def_case(tb.clear());
}