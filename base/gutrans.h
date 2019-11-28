//
// Created by meshate on 2019/11/28.
//

#ifndef NOGU_GUTRANS_H
#define NOGU_GUTRANS_H

#include <cstdint>

namespace nogu {
    int u32toh(uint32_t _num, char* _buf);
    int u64toh(uint64_t _num, char* _buf);

    int u32toa(uint32_t _num, char* _buf);
    int u64toa(uint64_t _num, char* _buf);

    int i32toa(int32_t _num, char* _buf);
    int i64toa(int64_t _num, char* _buf);
}

#endif //NOGU_GUTRANS_H
