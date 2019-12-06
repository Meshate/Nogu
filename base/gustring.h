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

        void swap(gustring& s) {
            if (&s != this) {
                _Mem* p = _p;
                _p = s._p;
                s._p = p;
            }
        }

        void swap(gustring&& s) {
            s.swap(*this);
        }

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

        gustring slice(size_t b = 0, size_t e = 0);

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

    inline bool operator==(const gustring &first, const gustring &second) {
        if (first.size() != second.size()) return false;
        return first.empty() || memcmp(first.data(), second.data(), first.size()) == 0;
    }

    inline bool operator==(const gustring &first, const char *second) {
        if (first.size() != strlen(second)) return false;
        return first.empty() || memcmp(first.data(), second, first.size()) == 0;
    }

    inline bool operator==(const char *first, const gustring &second) {
        return second == first;
    }

    inline bool operator!=(const gustring &first, const gustring &second) {
        return !(first == second);
    }

    inline bool operator!=(const gustring &first, const char *second) {
        return !(first == second);
    }

    inline bool operator!=(const char *first, const gustring &second) {
        return !(first == second);
    }

    inline gustring operator+(const gustring &first, const gustring &second) {
        return gustring(first.size() + second.size() + 1).append(first).append(second);
    }

    inline gustring operator+(const gustring &first, const char *second) {
        return gustring(first.size() + strlen(second) + 1).append(first).append(second);
    }

    inline gustring operator+(const char *first, const gustring &second) {
        return gustring(strlen(first) + second.size() + 1).append(first).append(second);
    }

    inline gustring operator+(const gustring &first, const char c) {
        return gustring(first.size() + 2).append(first).append(c);
    }

    inline gustring operator+(const char c, const gustring &second) {
        return gustring(second.size() + 2).append(c).append(second);
    }

    inline bool operator>(const gustring &first, const gustring &second) {
        if (first.size() > second.size()) {
            return second.empty() || memcmp(first.data(), second.data(), second.size()) >= 0;
        } else {
            return memcmp(first.data(), second.data(), first.size()) > 0;
        }
    }

    inline bool operator>(const gustring &first, const char *second) {
        if (first.size() > strlen(second)) {
            return strlen(second) == 0 || memcmp(first.data(), second, strlen(second)) >= 0;
        } else {
            return memcmp(first.data(), second, first.size()) > 0;
        }
    }

    inline bool operator<(const gustring &first, const gustring &second) {
        if (first.size() < second.size()) {
            return second.empty() || memcmp(first.data(), second.data(), second.size()) <= 0;
        } else {
            return memcmp(first.data(), second.data(), first.size()) < 0;
        }
    }

    inline bool operator<(const gustring &first, const char *second) {
        if (first.size() < strlen(second)) {
            return strlen(second) == 0 || memcmp(first.data(), second, strlen(second)) <= 0;
        } else {
            return memcmp(first.data(), second, first.size()) < 0;
        }
    }

    inline bool operator>(const char *first, const gustring &second) {
        return second < first;
    }

    inline bool operator<(const char *first, const gustring &second) {
        return second > first;
    }



    inline std::ostream &operator<<(std::ostream &os, const gustring &s) {
        return os << s.c_str();
    }
}

#endif //NOGU_GUSTRING_H
