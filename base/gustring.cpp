//
// Created by meshate on 2019/12/1.
//

#include "gustring.h"

namespace nogu {

    void gustring::_Init(size_t cap, size_t size) {
        _p = (_Mem *) malloc(sizeof(_Mem) + cap);
        _p->size = size;
        _p->cap = cap;
        _p->ref_count = 1;
    }

    void gustring::_Reserve(size_t cap) {
        if (_p->cap < cap) {
            if (_p->ref_count == 1) {
                _p = (_Mem *) realloc(_p, cap + sizeof(_Mem));
                assert(_p);
                _p->cap = cap;
            } else {
                --_p->ref_count;
                char *s = _p->m;
                this->_Init(cap, _p->size);
                memcpy(_p->m, s, _p->size);
            }
        }
    }

    gustring::gustring(const char *s) {
        if (!*s) {
            _p = 0;
            return;
        }
        size_t n = strlen(s);
        this->_Init(n + 1, n);
        memcpy(_p->m, s, n + 1);
    }

    gustring &gustring::operator=(const char *s) {
        if (!*s) {
            this->clear();
            return *this;
        }

        size_t n = strlen(s);
        if (_p) {
            this->_Reserve(n + 1);
        } else {
            this->_Init(n + 1, n);
        }
        memcpy(_p->m, s, n + 1);
        return *this;
    }

    void gustring::_Ensure(size_t n) {
        if (_p->cap < _p->size + n) {
            this->_Reserve(_p->cap * 2 + n);
        }
    }

    gustring &gustring::append(char c) {
        _p ? this->_Ensure(1) : this->_Init(8);
        _p->m[_p->size++] = c;
        return *this;
    }

    gustring &gustring::append(char c, size_t n) {
        _p ? this->_Ensure(n) : this->_Init(n + 1);
        memset(_p->m + _p->size, c, n);
        _p->size += (unsigned) n;
        return *this;
    }

    gustring &gustring::append(const char *s) {
        size_t len = strlen(s);
        _p ? this->_Ensure(len) : this->_Init(len + 1);
        memcpy(_p->m + _p->size, s, len);
        _p->size += len;
        return *this;
    }

    gustring &gustring::append(const gustring &s) {
        if (&s != this) {
            return this->append(s.data());
        } else if (_p) {
            this->_Reserve(_p->size * 2);
            memcpy(_p->m + _p->size, _p->m, _p->size);
            _p->size *= 2;
        }
        return *this;
    }

    bool gustring::match(const gustring &s) const {
//        mector<mector<bool>> dp(_p->size + 1, mector<bool>(s.size() + 1, 0));
        bool dp[_p->size + 1][s.size() + 1];
        memset(dp, 0, sizeof(dp));
        dp[_p->size][s.size()] = true;
        for (int i = _p->size; i >= 0; --i) {
            for (int j = s.size() - 1; j >= 0; --j) {
                bool match = false;
                if (i < _p->size && (_p->m[i] == s[j] || s[j] == '.')) {
                    match = true;
                }
                if (j + 1 < s.size() && s[j + 1] == '*') {
                    dp[i][j] = dp[i][j + 2] || (match && dp[i + 1][j]);
                } else {
                    dp[i][j] = match && dp[i + 1][j + 1];
                }
            }
        }
        return dp[0][0];
    }

    gustring gustring::slice(size_t b, size_t e) {
        if (e == 0)e=_p->size;
        char ret[_p->size+1];
        memcpy(ret,_p->m+b,e-b);
        return gustring(ret);
    }


}//nogu
