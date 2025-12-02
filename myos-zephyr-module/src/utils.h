#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>



#define UTILS_UINT(bits)          UTILS_UINT_EXPAND(bits)
#define UTILS_UINT_EXPAND(bits)   UTILS_UINT_##bits
#define UTILS_UINT_8          uint8_t
#define UTILS_UINT_16         uint16_t
#define UTILS_UINT_32         uint32_t
#define UTILS_UINT_64         uint64_t

#define UTILS_INT(bits)          UTILS_INT_EXPAND(bits)
#define UTILS_INT_EXPAND(bits)   UTILS_INT_##bits
#define UTILS_INT_8           int8_t
#define UTILS_INT_16          int16_t
#define UTILS_INT_32          int32_t
#define UTILS_INT_64          int64_t

#endif /* UTILS_H_ */
