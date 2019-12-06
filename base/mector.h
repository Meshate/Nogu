//
// Created by meshate on 2019/11/28.
//

#ifndef NOGU_MECTOR_H
#define NOGU_MECTOR_H

#include <initializer_list>
#include <ostream>
#include <cstdlib>
//TODO fix cant work with mector<mector>
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

        mector(std::initializer_list<T> items) {
            this->_Init(items.size(), items.size());
            this->_Set_value(items);
        }

        mector(T *begin, T *end) {
            size_t len = end - begin;
            this->_Init(len, len);
            this->_Set_value(begin, len);
        }

        mector(const mector &other) {
            this->_Init(other._p->cap, other._p->size);
            this->_Set_value(other._p->m, other._p->size * sizeof(T));
        }

        mector(mector &&other) noexcept {
            _p = other._p;
            other._p = nullptr;
        }

        ~mector() {
            if (_p)free(_p);
        }

        T &operator[](size_t n) const { return _p->m[n]; }

        mector<T> &operator=(const mector &other) {
            if (other != *this) {
                this->_Init(other._p->cap, other._p->size);
                this->_Set_value(other._p->m, other._p->size * sizeof(T));
            }
            return *this;
        }

        mector<T> &operator=(mector &&other) noexcept {
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

        T &front() const {
            return _p->m[0];
        }

        T &back() const {
            return _p->m[_p->size - 1];
        }

        void clear() {
            if (_p)_p->size = 0;
        }

        const T *data() const {
            return _p ? _p->m : 0;
        }

        void push_back(const T &item) {
            if (!_p)this->_Init(1);
            if (_p->size == _p->cap)this->_Reserve(_p->cap * 2);
            _p->m[_p->size] = item;
            _p->size++;
        }

        void push_back(T &&item) {
            if (!_p)this->_Init(1);
            if (_p->size == _p->cap)this->_Reserve(_p->cap * 2);
            _p->m[_p->size] = item;
            _p->size++;
            item = 0;
        }

        template<typename... Args>
        void emplace_back(Args &&... args) {
            if (!_p)this->_Init(1);
            if (_p->size == _p->cap)this->_Reserve(_p->cap * 2);
            ::new(_p->m + _p->size) T(std::forward<Args>(args)...);
            _p->size++;
        }

        iterator insert(const iterator pos, const T &x) {
            if (!_p)this->_Init(1);
            if (_p->size == _p->cap)this->_Reserve(_p->cap * 2);
            memcpy(pos + 1, pos, end() - pos);
            _p->size++;
            *pos = x;
            return pos;
        }

        iterator insert(const iterator pos, T &&x) {
            if (!_p)this->_Init(1);
            if (_p->size == _p->cap)this->_Reserve(_p->cap * 2);
            memmove(pos + 1, pos, (end() - pos) * sizeof(T));
            _p->size++;
            *pos = x;
            x = 0;
            return pos;
        }

        //TODO 找到erase操作比std::vector慢的原因并解决
        iterator erase(const iterator pos) {
            if (!_p)return nullptr;
            memmove(pos, pos + 1, (end() - pos - 1) * sizeof(T));
            _p->size--;
            return pos;
        }

        iterator erase(const iterator begin, const iterator end) {
            if (!_p)return nullptr;
            memmove(begin, end, (end() - end) * sizeof(T));
            _p->size -= end - begin;
            return begin;
        }

        void pop_back() {
            if (_p)_p->size--;
        }

        void swap(mector &other) {
            if (&other != this) {
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

        inline void _Set_value(size_t begin, size_t end, T item) {
            for (int i = begin; i < end; ++i) {
                _p->m[i] = item;
            }
        }

        inline void _Set_value(T items[], size_t n) {
            memcpy(_p->m, items, n);
        }

        inline void _Set_value(std::initializer_list<T> items) {
            memcpy(_p->m, items.begin(), sizeof(T) * items.size());
        }

    private:
        struct _Mem {
            size_t cap;
            size_t size;
            T m[];
        };

        _Mem *_p;
    };

    template<typename T>
    void mector<T>::_Init(size_t cap, size_t size) {
        _p = (_Mem *) malloc(sizeof(_Mem) + cap * sizeof(T));
        _p->size = size;
        _p->cap = cap;
    }

    template<typename T>
    void mector<T>::_Reserve(size_t cap) {
        if (_p->cap < cap) {
            _p = (_Mem *) realloc(_p, sizeof(_Mem) + cap * sizeof(T));
            assert(_p);
            _p->cap = cap;
        }
    }

    template<typename T>
    inline std::ostream &operator<<(std::ostream &os, const mector<T> &mec) {
        os << "cap: " << mec.capacity() << '\n';
        os << "size: " << mec.size() << '\n';
        os << "data:{";
        for (auto i = mec.begin(); i != mec.end(); ++i) {
            os << " " << *i;
        }
        os << " }";
        return os;
    }
} //nogu



#endif //NOGU_MECTOR_H
