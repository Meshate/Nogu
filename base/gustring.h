//
// Created by meshate on 2019/12/1.
//

#ifndef NOGU_GUSTRING_H
#define NOGU_GUSTRING_H

#include <ostream>
#include <cstdlib>

namespace nogu {
    class gustring {
    public:
        typedef char *iterator;
    public:
        gustring() : _p(0) {}

        gustring(size_t cap) {
            this->_Init(cap);
            this->_Set_value(0, cap, 0);
        }

        gustring(size_t cap, char item) {
            this->_Init(cap, cap);
            this->_Set_value(0, cap, item);
        }


        gustring(char *begin, char *end) {
            size_t len = end - begin;
            this->_Init(len, len);
            this->_Set_value(begin, len);
        }

        gustring(const gustring &other) {
            this->_Init(other._p->cap, other._p->size);
            this->_Set_value(other._p->m, other._p->size * sizeof(char));
        }

        gustring(gustring &&other) noexcept {
            _p = other._p;
            other._p = nullptr;
        }

        ~gustring() {
            if (_p)free(_p);
        }

        char &operator[](size_t n) const { return *(_p->m + n); }

        gustring &operator=(const gustring &other);

        gustring &operator=(gustring &&other) noexcept {
            _p = other._p;
            other._p = nullptr;
        }

        const int size() const {
            return _p ? _p->size : 0;
        }

        const int capacity() const {
            return _p ? _p->cap : 0;
        }

        iterator begin() const {
            return _p ? _p->m : 0;
        }

        iterator end() const {
            return _p ? (_p->m + _p->size) : 0;
        }

        bool empty() const {
            return !_p || _p->size == 0;
        }

        void clear() {
            if (_p)_p->size = 0;
        }

        const char *data() const {
            if (_p)return _p->m;
        }


        void swap(gustring &other) {
            if (other != *this) {
                _Mem *p = _p;
                _p = other._p;
                other._p = p;
            }
        }

        void reserve(size_t n) {
            _p ? this->_Reserve(n) : this->_Init(n);
        }

        void resize(size_t n) {
            _p ? this->_Reserve(n) : this->_Init(n);
            _p->size = n;
        }

    private:
        void _Init(size_t cap, size_t size = 0);

        void _Reserve(size_t cap);

        inline void _Set_value(size_t begin, size_t end, char item) {
            for (int i = begin; i < end; ++i) {
                *(_p->m + i) = item;
            }
        }

        inline void _Set_value(char items[], size_t n) {
            memcpy(_p->m, items, n);
        }

    private:
        struct _Mem {
            size_t cap;
            size_t size;
            size_t ref_count;
            char m[];
        };

        _Mem *_p;
    };
}

#endif //NOGU_GUSTRING_H
