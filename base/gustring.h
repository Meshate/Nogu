//
// Created by meshate on 2019/12/1.
//

#ifndef NOGU_GUSTRING_H
#define NOGU_GUSTRING_H

#include <ostream>
#include <cstdlib>
#include <cstring>
#include "mector.h"

namespace nogu {
    class gustring {
    public:
        typedef char *iterator;
    public:
        gustring() : _p(0) {}

        explicit gustring(size_t cap) {
            this->_Init(cap);
        }

        gustring(size_t cap, char item) {
            this->_Init(cap + 1, cap);
            memset(_p->m, item, cap);
        }

        gustring(char item, size_t cap) {
            this->_Init(cap + 1, cap);
            memset(_p->m, item, cap);
        }

        gustring(const char *s);

        gustring(const gustring &other) {
            _p = other._p;
            _p->ref_count++;
        }

        gustring(gustring &&other) noexcept {
            _p = other._p;
            other._p = nullptr;
        }

        ~gustring() {
            if (!_p) return;
            if (--_p->ref_count == 0) free(_p);
            _p = 0;
        }

        char &operator[](size_t n) const {
            return _p->m[n];
        }

        gustring operator()(size_t b = 0, size_t e = 0x7fffffff);

        gustring &operator=(const gustring &other) {
            if (&other != this) {
                if (_p && --_p->ref_count == 0) free(_p);
                _p = other._p;
                if (_p) _p->ref_count++;
            }
            return *this;
        }

        gustring &operator=(gustring &&other) noexcept {
            if (_p && --_p->ref_count == 0) free(_p);
            _p = other._p;
            other._p = nullptr;
            return *this;
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

        char &front() const {
            return _p->m[0];
        }

        char &back() const {
            return _p->m[_p->size - 1];
        }

        void clear() {
            if (_p)_p->size = 0;
        }

        const char *data() const {
            return _p ? _p->m : 0;
        }

        const char *c_str() const {
            if (!_p) return nullptr;
            const_cast<gustring *>(this)->_Reserve(_p->size + 1);
            if (_p->m[_p->size] != '\0') _p->m[_p->size] = '\0';
            return _p->m;
        }

        gustring &append(char c);

        gustring &append(char c, size_t n);

        gustring &append(size_t n, char c) {
            return append(c, n);
        }

        gustring &append(const char *s);

        gustring &append(const gustring &s);

//        void swap(gustring &other) {
//            if (other != *this) {
//                _Mem *p = _p;
//                _p = other._p;
//                other._p = p;
//            }
//        }

        void reserve(size_t n) {
            _p ? this->_Reserve(n) : this->_Init(n);
        }

        void resize(size_t n) {
            _p ? this->_Reserve(n) : this->_Init(n);
            _p->size = n;
        }

        // . match any one character
        // * match any number of previous character
        bool match(const gustring &s) const;

        gustring &operator=(const char *s);

    private:
        void _Init(size_t cap, size_t size = 0);

        void _Reserve(size_t cap);

        void _Ensure(size_t n);

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
