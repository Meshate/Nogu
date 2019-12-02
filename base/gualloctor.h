//
// Created by meshate on 2019/12/1.
//

#ifndef NOGU_GUALLOCTOR_H
#define NOGU_GUALLOCTOR_H

#include <cstdio>
#include <cstdlib>
#include <mutex>

#define THREADS_SAFE

namespace nogu {
    template<typename T, size_t block_size = 4096>
    class gulloctor {
    public:
        template<typename U>
        struct rebind {
            typedef gulloctor<U> other;
        };

        gulloctor() noexcept : _current_block(nullptr),
                               _current_node(nullptr),
                               _free_block(nullptr),
                               _free_node(nullptr) {
            static_assert(block_size >= 4 * sizeof(T), "block size too small");
        }

        gulloctor(const gulloctor &other) = delete;

        gulloctor(gulloctor &&other) noexcept {
#ifdef THREADS_SAFE
            std::lock_guard<std::mutex> lk(other._mtx);
#endif
            _current_block = other._current_block;
            _current_node = other._current_node;
            _free_block = other._free_block;
            _free_node = other._free_node;
            other._free_node = nullptr;
            other._free_block = nullptr;
            other._current_node = nullptr;
            other._current_block = nullptr;
        }

        ~gulloctor() noexcept {
#ifdef THREADS_SAFE
            std::lock_guard<std::mutex> lk(_mtx);
#endif
            while (_current_block != nullptr) {
                auto next = _current_block->_prev;
                free(_current_block);
                _current_block = next;
            }
        }

        gulloctor &operator=(const gulloctor &other) = delete;

        gulloctor &operator=(gulloctor &&other) noexcept {
#ifdef THREADS_SAFE
            std::lock_guard<std::mutex> lk(other._mtx);
#endif
            _current_block = other._current_block;
            _current_node = other._current_node;
            _free_block = other._free_block;
            _free_node = other._free_node;
            other._free_node = nullptr;
            other._free_block = nullptr;
            other._current_node = nullptr;
            other._current_block = nullptr;
        }

        T *allocate(size_t n);

        void deallocate(T *p, size_t n);

        template<typename U>
        void destroy(U *p) {
#ifdef THREADS_SAFE
            std::lock_guard<std::mutex> lk(_mtx);
#endif
            if (p != nullptr) {
                _Node *it = this->_Get_node(p);
                it->~_Node();
            }
        }

        template<typename U, typename... Args>
        void construct(U *p, Args &&... args) {
            ::new(p) T(std::forward<Args>(args)...);
        }

    private:
        struct _Node {
            _Node *_next = nullptr;
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
#ifdef THREADS_SAFE
        std::mutex _mtx;
#endif

        void _Allocate_block();

        inline _Node *_Get_node(T *p) {
            return (_Node *) ((ptrdiff_t *) p - 1);
        }
    };

    template<typename T, size_t block_size>
    void gulloctor<T, block_size>::_Allocate_block() {
        if (_free_block == nullptr) {
            _Block *new_block = (_Block *) malloc(sizeof(_Block) + block_size);
            new_block->_prev = _current_block;
            _current_block = new_block;
            for (int i = 0; i < block_size / sizeof(_Node) - 2; ++i) {
                _current_block->_n[i]._next = _current_block->_n + i + 1;
            }
            _current_block->_n[block_size / sizeof(_Node) - 1]._next = nullptr;
            if (_current_node)_free_node->_next = _current_block->_n;
            else _current_node = _free_node = _current_block->_n;
        }
    }

    template<typename T, size_t block_size>
    T *gulloctor<T, block_size>::allocate(size_t n) {
#ifdef THREADS_SAFE
        std::lock_guard<std::mutex> lk(_mtx);
#endif
        _current_node = _free_node;
        if (_current_block == nullptr && _current_node == nullptr) {
            this->_Allocate_block();
        }
        for (int i = 0; i < (int) n; ++i) {
            if (_free_node->_next == nullptr)this->_Allocate_block();
            _free_node = _free_node->_next;
        }
        return &_current_node->data;
    }

    template<typename T, size_t block_size>
    void gulloctor<T, block_size>::deallocate(T *p, size_t n) {
#ifdef THREADS_SAFE
        std::lock_guard<std::mutex> lk(_mtx);
#endif
        if (p != nullptr) {
            _Node *first = this->_Get_node(p);
            _Node *last = this->_Get_node(p);
            for (int i = 1; i < (int) n; ++i) {
                if (last->_next != nullptr)last = last->_next;
                else break;
            }
            last->_next = _free_node;
            _free_node = first;
        }
    }
}//nogu

#endif //NOGU_GUALLOCTOR_H
