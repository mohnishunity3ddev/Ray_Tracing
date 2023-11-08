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

#define internal static

// Constants
const f64 Infinity = std::numeric_limits<f64>::infinity();
const f64 pi = 3.1415926535897932385;

// Utility Functions
inline f64
Deg2Rad(const f64 Degrees)
{
    f64 Result = (Degrees*pi) / 180.0;
    return Result;
}

// CPP Random sometimes gives faulty results.
#define USE_CPP_RANDOM 0

#if USE_CPP_RANDOM
#include <random>
inline f64
Rand01()
{
    static std::uniform_real_distribution<f64> Distribution(0., 1.);
    static std::mt19937 Generator;
    f64 Result = Distribution(Generator);
    return Result;
}
#else
inline f64
Rand01()
{
    f64 Result = rand() / (RAND_MAX + 1.);
    return Result;
}
#endif

inline f64
RandRange(f64 Min, f64 Max)
{
    f64 Result = Min + ((Max - Min)*Rand01());
    return Result;
}

template <typename T>
inline T
Rand01Generic()
{
    T Result = (T)rand() / ((T)RAND_MAX + (T)1);
    return Result;
}

inline i32
RandomRangeInt(i32 A, i32 B)
{
    i32 Result = static_cast<i32>(RandRange(A, B+1));
    
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

template<typename T>
void
Swap(T &A, T &B)
{
    T Temp = A;
    A = B;
    B = Temp;
}

#ifdef _MSC_VER
#define SHU_EXPORT __declspec(dllexport)
#elif defined(__clang__)
#define SHU_EXPORT __attribute__((visibility("default")))
#endif

#define DEFINES_H
#endif