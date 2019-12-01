//
// Created by meshate on 2019/12/1.
//

#ifndef NOGU_GUALLOCTOR_H
#define NOGU_GUALLOCTOR_H

#include <cstdio>
#include <utility>
#include <mutex>

namespace nogu {
    template<typename T, size_t block_size = 4096>
    class gulloctor {
    public:
        gulloctor() : _current_block(nullptr),
                      _current_node(nullptr),
                      _free_block(nullptr),
                      _free_node(nullptr) {}

        gulloctor(const gulloctor &other) = delete;

        ~gulloctor();

        gulloctor &operator=(const gulloctor &other) = delete;

        T *allocate(size_t n);

        void deallocate(T *p, size_t n);

        void destroy(T *p);

        template<typename... Args>
        void construct(T *p, Args &&... args) {
            ::new(p) T(std::forward<Args>(args)...);
        }

    private:
        struct _Node {
            _Node *_next;
            T data;
        };
        struct _Block {
            _Block *_prev;
            _Node _n[];
        };

        _Block *_free_block;
        _Block *_current_block;
        _Node *_current_node;
        _Node *_free_node;

        std::mutex _mtx;

        void _Allocate_block();

        inline _Node *_Get_node(T *x) {
            return (_Node *) ((ptrdiff_t *) x - 1);
        }
    };

    template<typename T, size_t block_size>
    void gulloctor<T, block_size>::_Allocate_block() {
        std::lock_guard<std::mutex> lk(_mtx);
        if (_free_block == nullptr) {
            _Block *new_block = (_Block *) malloc(sizeof(_Block) + block_size);
            new_block->_prev = _current_block;
            _current_block = new_block;
            for (int i = 0; i < block_size / sizeof(T) - 1; ++i) {
                _current_block->_n[i]._next = _current_block->_n + i + 1;
            }
            _current_block->_n[block_size / sizeof(T)]._next = nullptr;
            if (_current_node)_free_node->_next = _current_block->_n;
            else _current_node = _free_node = _current_block->_n;
        }
    }

    template<typename T, size_t block_size>
    T *gulloctor<T, block_size>::allocate(size_t n) {
        std::lock_guard<std::mutex> lk(_mtx);
        if (_current_block == nullptr && _current_node == nullptr) {
            this->_Allocate_block();
        }
        for (int i = 0; i < (int) n; ++i) {
            if (_free_node->_next == nullptr)this->_Allocate_block();
            _free_node = _free_node->_next;
        }
    }

    template<typename T, size_t block_size>
    void gulloctor<T, block_size>::deallocate(T *p, size_t n) {

    }

    template<typename T, size_t block_size>
    void gulloctor<T, block_size>::destroy(T *p) {
        std::lock_guard<std::mutex> lk(_mtx);
        if (p == nullptr)return;
        _Node* it = this->_Get_node(p);
        it->_next = _free_node;
        _free_node = it;
    }


}//nogu

#endif //NOGU_GUALLOCTOR_H
