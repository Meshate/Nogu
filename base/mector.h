//
// Created by meshate on 2019/11/28.
//

#ifndef NOGU_MECTOR_H
#define NOGU_MECTOR_H

#include <initializer_list>
#include <ostream>
#include <cstdlib>

namespace nogu {
    template<typename T>
    class mector {
    public:
        typedef T *iterator;
    public:
        mector() : _p(0) {}

        mector(size_t cap) {
            this->_Init(cap);
            this->_Set_value(0, cap, 0);
        }

        mector(size_t cap, T item) {
            this->_Init(cap, cap);
            this->_Set_value(0, cap, item);
        }

        mector(std::initializer_list<T> &&items) {
            this->_Init(items.size(), items.size());
            this->_Set_value(items);
            items = 0;
        }

        ~mector() {
            if (!_p)return;
            if (_p->ref_count == 1)free(_p);
            _p = 0;
        }

        T &operator[](size_t n) const { return *(_p->m + n); }

        size_t size() const { return _p ? _p->size : 0; }

        size_t capacity() const { return _p ? _p->cap : 0; }

        iterator begin() const { return _p ? _p->m : 0; }

        iterator end() const { return _p ? (_p->m + _p->size) : 0; }

        bool empty() const { return !_p || _p->size == 0; }

        void push_back(const T &item) {
            if (!_p)this->_Init(1);
            else if (_p->size == _p->cap)this->_Reserve(_p->cap * 2);
            *(_p->m + _p->size) = item;
            _p->size++;
        }

        void push_back(T &&item) {
            if (!_p)this->_Init(1);
            else if (_p->size == _p->cap)this->_Reserve(_p->cap * 2);
            *(_p->m + _p->size) = item;
            _p->size++;
            item = 0;
        }

        void pop() {
            if (_p)_p->size--;
        }

    private:
        void _Init(size_t cap, size_t size = 0);

        void _Reserve(size_t cap);

        inline void _Set_value(size_t begin, size_t end, T item) {
            for (int i = begin; i < end; ++i) {
                *(_p->m + i) = item;
            }
        }

        inline void _Set_value(T items[],size_t n) {
            memcpy(_p->m, items, n);
        }

    private:
        struct _Mem {
            size_t cap;
            size_t size;
            size_t ref_count;
            T m[];
        };

        _Mem *_p;
    };

    template<typename T>
    void mector<T>::_Init(size_t cap, size_t size) {
        _p = (_Mem *) malloc(sizeof(_Mem) + cap * sizeof(T));
        _p->size = size;
        _p->cap = cap;
        _p->ref_count = 1;
    }

    template<typename T>
    void mector<T>::_Reserve(size_t cap) {
        if (_p->cap < cap) {
            if (_p->ref_count == 1) {
                _p = (_Mem *) realloc(_p, sizeof(_Mem) + cap * sizeof(T));
                assert(_p);
                _p->cap = cap;
            } else {
                --_p->ref_count;
                T *t = _p->m;
                size_t size = _p->size;
                this->_Init(cap, _p->size);
                this->_Set_value(t,size);
            }
        }
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &out, const mector<T> &mec) {
        out << "cap: " << mec.capacity() << '\n';
        out << "size: " << mec.size() << '\n';
        out << "data:{";
        for (auto i = mec.begin(); i != mec.end(); ++i) {
            out << " " << *i;
        }
        out<<" }";
        return out;
    }
} //nogu



#endif //NOGU_MECTOR_H
