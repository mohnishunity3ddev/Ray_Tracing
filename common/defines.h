#if !defined(DEFINES_H)

#include <stdint.h>
#include <climits>
#include <memory>

typedef int8_t b8;
typedef int16_t b16;
typedef int32_t b32;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef float f32;
typedef double f64;

// Constants
const f64 Infinity = std::numeric_limits<f64>::infinity();
const f64 pi = 3.1415926535897932385;

// Utility Functions
inline f64 Deg2Rad(const f64 Degrees)
{
    f64 Result = (Degrees*pi) / 180.0;
    return Result;
}

#define ABSOLUTE(Val) (Val) < 0 ? -(Val) : (Val)

#define ASSERT(Expression)                                                     \
    if (!(Expression))                                                         \
    {                                                                          \
        *((int volatile *)0) = 0;                                              \
    }

#define OFFSET_OF(Var, Member) (u64)(&(((Var *)0)->Member))

#define MAX(A, B) (A) > (B) ? A : B
#define MIN(A, B) (A) < (B) ? A : B

#ifdef _MSC_VER
#define SHU_EXPORT __declspec(dllexport)
#elif defined(__clang__)
#define SHU_EXPORT __attribute__((visibility("default")))
#endif

#define DEFINES_H
#endif