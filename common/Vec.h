#if !defined(VEC_H)
#include "defines.h"

template <typename T>
struct vec2
{
    union
    {
        struct { T x, y; };
        struct { T u, v; };
        struct { T w, h; };
        f32 E[2];
    };

    inline vec2<T> operator+=(const vec2<T>& A);
    inline vec2<T> operator-=(const vec2<T>& A);
    inline vec2<T> operator*=(T A);
    inline vec2<T> operator/=(const vec2<T>& A);
    inline vec2<T> operator/=(T A);
    inline T& operator[](size_t Index);
    inline T Dot(const vec2<T>& A);
    inline T SqMagnitude();
    inline T Magnitude();
};

typedef vec2<f64> vec2d;
typedef vec2<i32> vec2i;
typedef vec2<f32> vec2f;
typedef vec2<u32> vec2u;
#define Vec2f Vec2<f32>
#define Vec2d Vec2<f64>
#define Vec2u Vec2<u32>
#define Vec2i Vec2<i32>

template <typename T> SHU_EXPORT vec2<T> Vec2();
template <typename T> SHU_EXPORT vec2<T> Vec2(T A);
template <typename T> SHU_EXPORT vec2<T> Vec2(T x, T y);
template <typename T> SHU_EXPORT vec2<T> MakeVec2(const T *const Ptr);
template <typename T> SHU_EXPORT T Dot(const vec2<T>& A, const vec2<T>& B);
template <typename T> SHU_EXPORT vec2<T> operator+(const vec2<T>& A, const vec2<T>& B);
template <typename T> SHU_EXPORT vec2<T> operator-(const vec2<T>& A, const vec2<T>& B);
template <typename T> SHU_EXPORT vec2<T> operator*(const vec2<T>& A, T B);
template <typename T> SHU_EXPORT vec2<T> operator/(const vec2<T>& A, const vec2<T>& B);
template <typename T> SHU_EXPORT vec2<T> operator/(const vec2<T>& A, T B);
template <typename T> SHU_EXPORT T SqMagnitude(const vec2<T> &A);
template <typename T> SHU_EXPORT T Magnitude(const vec2<T> &A);
template <typename T> SHU_EXPORT vec2f Normalize(const vec2<T> &A);

// ----------------------------------------------------------------------------------------------------------------

template<typename T>
struct vec3
{
  public:
    static inline vec3<T> RandomInUnitSphere();

  public:
    union
    {
        struct { T x, y, z; };
        struct { T u, v, w_; };
        struct { T r, g, b; };
        struct { T w, h, d; };
        struct { vec2<T> xy; T Unused_0; };
        struct { T Unused_1; vec2<T> yz; };
        struct { vec2<T> uv; T Unused_2; };
        struct { T Unused_3; vec2<T> vw; };
        struct { vec2<T> rg; T Unused_4; };
        struct { T Unused_5; vec2<T> gb; };
        T E[3];
    };

    inline vec3<T> operator+=(const vec3<T>& A);
    inline vec3<T> operator-=(const vec3<T>& A);
    inline vec3<T> operator*=(T A);
    inline vec3<T> operator/=(const vec3<T>& A);
    inline vec3<T> operator/=(T A);
    inline T& operator[](size_t Index);
    inline T SqMagnitude() const;
    inline T Magnitude() const;
    inline T Dot(const vec3<T> &A);
    inline vec3<T> Cross(const vec3<T> &A);
    inline b32 NearZero() const;

    static inline vec3<T> Rand01();
    static inline vec3<T> RandRange(T Min, T Max);
    static inline vec3<T> One();
    static inline vec3<T> RandomUnitVector();
    static inline vec3<T> RandomInUnitHemisphere(const vec3<T> &Normal);
    static inline vec3<T> RandomInUnitDisk();
};
typedef vec3<i32> vec3i;
typedef vec3<f32> vec3f;
typedef vec3<f64> vec3d;
typedef vec3<u32> vec3u;
#define Vec3d Vec3<f64>
#define Vec3f Vec3<f32>
#define Vec3u Vec3<u32>
#define Vec3i Vec3<i32>

template <typename T> SHU_EXPORT vec3<T> Vec3();
template <typename T> SHU_EXPORT vec3<T> Vec3(T A);
template <typename T> SHU_EXPORT vec3<T> Vec3(T x, T y, T z);
template <typename T> SHU_EXPORT vec3<T> MakeVec3(const T *const Ptr);
template <typename T> SHU_EXPORT vec3<T> Vec3(const vec2<T>& xy, T z);
template <typename T> SHU_EXPORT vec3<T> ToVec3(const vec2<T>& A);
template <typename T> SHU_EXPORT vec2<T> ToVec2(const vec3<T>& A);
template <typename T> SHU_EXPORT T Dot(const vec3<T>& A, const vec3<T>& B);
template <typename T> SHU_EXPORT vec3<T> Cross(const vec3<T>& A, const vec3<T>& B);
template <typename T> SHU_EXPORT vec3<T> operator+(const vec3<T>& A, const vec3<T>& B);
template <typename T> SHU_EXPORT vec3<T> operator-(const vec3<T>& A, const vec3<T>& B);
template <typename T> SHU_EXPORT vec3<T> operator-(const vec3<T>& A);
template <typename T, typename S> SHU_EXPORT vec3<T> operator*(S B, const vec3<T> &A);
template <typename T, typename S> SHU_EXPORT vec3<T> operator*(const vec3<T> &A, S B);
template <typename T> SHU_EXPORT vec3<T> operator*(T B, const vec3<T> &A);
template <typename T> SHU_EXPORT vec3<T> operator*(const vec3<T>& A, T B);
template <typename T> SHU_EXPORT vec3<T> operator*(const vec3<T>& A, const vec3<T>& B);
template <typename T> SHU_EXPORT vec3<T> operator/(const vec3<T>& A, const vec3<T>& B);
template <typename T> SHU_EXPORT vec3<T> operator/(const vec3<T>& A, T B);
template <typename T> SHU_EXPORT vec3<T> operator/(const vec3<T>& A, const i32 B);
template <typename T> SHU_EXPORT T SqMagnitude(const vec3<T> &A);
template <typename T> SHU_EXPORT T Magnitude(const vec3<T> &A);

template <typename T> SHU_EXPORT vec3<T> Normalize(const vec3<T> &A);
template <typename T> SHU_EXPORT vec3<T> Reflect(const vec3<T> &Incident, const vec3<T> &Normal);
template <typename T> SHU_EXPORT vec3<T> Refract(const vec3<T> &Incident, const vec3<T> &Normal, const f64 N1ByN2);

template <typename T> internal T DotStatic(const vec3<T>& A, const vec3<T>& B);

// ----------------------------------------------------------------------------------------------------------------

template <typename T>
struct vec4
{
    union
    {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { T u, v, s, t; };
        struct { vec2<T> xy; vec2<T> zw; };
        struct { vec2<T> rg; vec2<T> ba; };
        struct { vec2<T> uv; vec2<T> st; };
        struct { vec3<T> xyz; T Reserved_0; };
        struct { vec3<T> rgb; T Reserved_1; };
        T E[4];
    };

    inline vec4<T> operator+=(const vec4<T>& A);
    inline vec4<T> operator-=(const vec4<T>& A);
    inline vec4<T> operator*=(T A);
    inline vec4<T> operator/=(const vec4<T>& A);
    inline vec4<T> operator/=(T A);
    inline T &operator[](size_t Index);
};

template <typename T> SHU_EXPORT vec4<T> Vec4();
template <typename T> SHU_EXPORT vec4<T> Vec4(T A);
template <typename T> SHU_EXPORT vec4<T> Vec4(T x, T y, T z, T w);
template <typename T> SHU_EXPORT vec4<T> MakeVec4(const T *const Ptr);
template <typename T> SHU_EXPORT T Dot(const vec4<T> &A, const vec4<T> &B);
template <typename T> SHU_EXPORT vec4<T> operator+(const vec4<T>& A, const vec4<T>& B);
template <typename T> SHU_EXPORT vec4<T> operator-(const vec4<T>& A, const vec4<T>& B);
template <typename T> SHU_EXPORT vec4<T> operator*(const vec4<T>& A, const vec4<T>& B);
template <typename T> SHU_EXPORT vec4<T> operator*(const vec4<T>& A, T B);
template <typename T> SHU_EXPORT vec4<T> operator/(const vec4<T>& A, const vec4<T>& B);
template <typename T> SHU_EXPORT vec4<T> operator/(const vec4<T>& A, T B);

typedef vec4<i32> vec4i;
typedef vec4<f32> vec4f;
typedef vec4<u32> vec4u;
#define Vec4f Vec4<f32>
#define Vec4u Vec4<u32>
#define Vec4i Vec4<i32>

#if 1
// ----------------------------------------------------------------------------------------------------------------
// Vec2
// ----------------------------------------------------------------------------------------------------------------
template <typename T>
vec2<T>
Vec2()
{
    vec2<T> Result = {(T)0, (T)0};
    return Result;
}

template <typename T>
vec2<T>
Vec2(T A)
{
    vec2<T> Result = {A, A};
    return Result;
}

template <typename T>
vec2<T>
MakeVec2(const T * const Ptr)
{
    vec2<T> Result = {};
    memcpy(Result.E, Ptr, sizeof(T)*2);
    return Result;
}


template <typename T>
vec2<T>
Vec2(T x, T y)
{
    vec2<T> Result;
    Result.x = x;
    Result.y = y;
    return Result;
}

template <typename T>
vec2<T>
operator+(const vec2<T>& A, const vec2<T>& B)
{
    vec2<T> Result = {A.x + B.x, A.y + B.y};
    return Result;
}

template <typename T>
vec2<T>
operator-(const vec2<T>& A, const vec2<T>& B)
{
    vec2<T> Result = {A.x - B.x, A.y - B.y};
    return Result;
}

template <typename T>
vec2<T>
operator*(const vec2<T>& A, T B)
{
    vec2<T> Result = {B * A.x, B * A.y};
    return Result;
}

template <typename T>
vec2<T>
operator/(const vec2<T>& A, const vec2<T>& B)
{
    vec2<T> Result;

    ASSERT(B.x != 0.0f && B.y != 0.0f);
    Result.x = A.x / B.x;
    Result.y = A.y / B.y;

    return Result;
}

template <typename T>
vec2<T>
operator/(const vec2<T>& A, T B)
{
    vec2<T> Result;
    ASSERT(B != 0.0f);

    Result.x = A.x / B;
    Result.y = A.y / B;

    return Result;
}

template <typename T>
vec2<T>
vec2<T>::operator+=(const vec2<T>& A)
{
    *this = *this + A;
    return *this;
}

template <typename T>
vec2<T>
vec2<T>::operator-=(const vec2<T>& A)
{
    *this = *this - A;
    return *this;
}

template <typename T>
vec2<T>
vec2<T>::operator*=(T A)
{
    *this = *this * A;
    return *this;
}

template <typename T>
vec2<T>
vec2<T>::operator/=(const vec2<T>& A)
{
    *this = *this / A;
    return *this;
}

template <typename T>
vec2<T>
vec2<T>::operator/=(T A)
{
    *this = *this / A;
    return *this;
}

template <typename T>
T&
vec2<T>::operator[](size_t Index)
{
    if(Index >= 2)
    {
        ASSERT(!"Index Out of Bounds");
    }

    return this->E[Index];
}

template <typename T>
T
vec2<T>::SqMagnitude()
{
    T Result = this->x * this->x + this->y * this->y;
    return Result;
}

template <typename T>
T
vec2<T>::Magnitude()
{
    T Result = sqrtf(this->x*this->x + this->y*this->y);
    return Result;
}

template <typename T>
T
SqMagnitude(const vec2<T> &A)
{
    T Result = A.x*A.x + A.y*A.y;
    return Result;
}

template <typename T>
T
Magnitude(const vec2<T> &A)
{
    T Result = sqrtf(A.x*A.x + A.y*A.y);
    return Result;
}

template <typename T>
vec2f
Normalize(const vec2<T> &A)
{
    f32 OneByMagnitude = 1.0f / (f32)A.Magnitude();
    vec2f Result = Vec2f((f32)(A.x*OneByMagnitude),
                        (f32)(A.y*OneByMagnitude));
    return Result;
}

template <typename T>
T
Dot(const vec2<T>& A, const vec2<T>& B)
{
    T Result = A.x*B.x + A.y*B.y + A.z*B.z;
    return Result;
}

template <typename T>
T
vec2<T>::Dot(const vec2<T>& A)
{
    T Result = A.x * this->x + A.y * this->y;
    return Result;
}

// ----------------------------------------------------------------------------------------------------------------
// Vec3
// ----------------------------------------------------------------------------------------------------------------
template <typename T>
vec3<T>
Vec3()
{
    vec3<T> Result = {(T)0, (T)0, (T)0};
    return Result;
}

template <typename T>
vec3<T>
Vec3(T A)
{
    vec3<T> Result = {A, A, A};
    return Result;
}

template <typename T>
vec3<T>
Vec3(T x, T y, T z)
{
    vec3<T> Result = {x, y, z};
    return Result;
}

template <typename T>
vec3<T>
MakeVec3(const T * const Ptr)
{
    vec3<T> Result = {};

    memcpy(Result.E, Ptr, sizeof(T) * 3);

    return Result;
}

template <typename T>
vec3<T>
Vec3(const vec2<T>& xy, T z)
{
    vec3<T> Result = Vec3(xy.x, xy.y, z);

    return Result;
}

template <typename T>
vec3<T>
ToVec3(const vec2<T>& A)
{
    vec3<T> Result = Vec3(A.x, A.y, 0.0f);
    return Result;
}

template <typename T>
vec2<T>
ToVec2(const vec3<T>& A)
{
    vec2<T> Result = Vec2(A.x, A.y);
    return Result;
}

template <typename T>
T
Dot(const vec3<T>& A, const vec3<T>& B)
{
    T Result = A.x*B.x + A.y*B.y + A.z*B.z;
    return Result;
}

template <typename T>
T
DotStatic(const vec3<T>& A, const vec3<T>& B)
{
    T Result = A.x*B.x + A.y*B.y + A.z*B.z;
    return Result;
}

template <typename T>
T
vec3<T>::Dot(const vec3<T> &A)
{
    T Result = A.x*this->x + A.y*this->y + A.z*this->z;
    return Result;
}

template <typename T>
vec3<T>
Cross(const vec3<T> &A, const vec3<T> &B)
{
    vec3<T> Result;
    Result.x = A.y*B.z - A.z*B.y;
    Result.y = A.z*B.x - A.x*B.z;
    Result.z = A.x*B.y - A.y*B.x;
    return Result;
}

template <typename T>
vec3<T>
vec3<T>::Cross(const vec3<T> &A)
{
    vec3<T> Result = Cross(*this, A);
    return Result;
}

// Returns true is the vector is zero or close to zero in all directions
template <typename T>
b32
vec3<T>::NearZero() const
{
    auto S = 1e-8;
    b32 Result = ((fabs(this->x) < S) &&
                  (fabs(this->y) < S) &&
                  (fabs(this->z) < S));

    return Result;
}

template <typename T>
vec3<T>
Reflect(const vec3<T> &V, const vec3<T> &N)
{
    vec3<T> Result;

    T d = Dot(V, N);
    // if V is moving IN to the surface with Normal N
    if(d < (T)0)
    {
        Result = V - 2*Dot(V, N)*N;
    }
    // if V is moving OUT of the surface with Normal N
    else if(d > (T)0)
    {
        Result = 2*Dot(V, N)*N - V;
    }

    return Result;
}

// NOTE: Proof is present in GoodNotes - Snell's Law Vector Form
// N1ByN2 is refractive Index of the incident medium divided by the refractive
// index of the refracting medium.
template <typename T>
vec3<T>
Refract(const vec3<T> &Incident, const vec3<T> &Normal, const f64 N1ByN2)
{
    f64 CosTheta = MIN(Dot(-Incident, Normal), 1.0);

    vec3<T> RPerpendicular = N1ByN2*(Incident + CosTheta*Normal);
    vec3<T> RParallel = -sqrt(ABSOLUTE(1.0 - RPerpendicular.SqMagnitude()))*Normal;

    vec3<T> Refracted = (RPerpendicular + RParallel);
    return Refracted;
}

template <typename T>
vec3<T>
operator+(const vec3<T>& A, const vec3<T>& B)
{
    vec3<T> Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;
    Result.z = A.z + B.z;

    return Result;
}

template <typename T>
vec3<T>
operator-(const vec3<T>& A, const vec3<T>& B)
{
    vec3<T> Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;
    Result.z = A.z - B.z;

    return Result;
}

template <typename T>
vec3<T>
operator-(const vec3<T>& A)
{
    vec3<T> Result;

    Result.x = -A.x;
    Result.y = -A.y;
    Result.z = -A.z;

    return Result;
}

template <typename T, typename S>
vec3<T>
operator*(S B, const vec3<T> &A)
{
    vec3<T> Result;

    Result.x = B * A.x;
    Result.y = B * A.y;
    Result.z = B * A.z;

    return Result;
}

template <typename T, typename S>
vec3<T>
operator*(const vec3<T>& A, S B)
{
    vec3<T> Result;

    Result.x = B * A.x;
    Result.y = B * A.y;
    Result.z = B * A.z;

    return Result;
}

template <typename T>
vec3<T>
operator*(const vec3<T>& A, T B)
{
    vec3<T> Result;

    Result.x = B * A.x;
    Result.y = B * A.y;
    Result.z = B * A.z;

    return Result;
}

template <typename T>
vec3<T>
operator*(const vec3<T> &A, const vec3<T> &B)
{
    vec3<T> Result;

    Result.x = B.x*A.x;
    Result.y = B.y*A.y;
    Result.z = B.z*A.z;

    return Result;
}

template <typename T>
vec3<T>
operator*(T B, const vec3<T> &A)
{
    vec3<T> Result;

    Result.x = B*A.x;
    Result.y = B*A.y;
    Result.z = B*A.z;

    return Result;
}

template <typename T>
vec3<T>
operator/(const vec3<T>& A, const vec3<T>& B)
{
    vec3<T> Result;

    ASSERT(B.x != 0.0f && B.y != 0.0f && B.z != 0.0f);

    Result.x = A.x / B.x;
    Result.y = A.y / B.y;
    Result.z = A.z / B.z;

    return Result;
}

template <typename T>
vec3<T>
operator/(const vec3<T>& A, T B)
{
    vec3<T> Result;

    ASSERT(B != 0.0f);

    Result.x = A.x / B;
    Result.y = A.y / B;
    Result.z = A.z / B;

    return Result;
}

template <typename T>
vec3<T>
operator/(const vec3<T> &A, const i32 B)
{
    vec3<T> Result;

    ASSERT(B != 0);

    Result.x = A.x / B;
    Result.y = A.y / B;
    Result.z = A.z / B;

    return Result;
}

template <typename T>
vec3<T>
vec3<T>::operator+=(const vec3<T>& A)
{
    *this = *this + A;
    return *this;
}

template <typename T>
vec3<T>
vec3<T>::operator-=(const vec3<T>& A)
{
    *this = *this - A;
    return *this;
}

template <typename T>
vec3<T>
vec3<T>::operator*=(T A)
{
    *this = *this * A;
    return *this;
}

template <typename T>
vec3<T>
vec3<T>::operator/=(const vec3<T>& A)
{
    *this = *this / A;
    return *this;
}

template <typename T>
vec3<T>
vec3<T>::operator/=(T A)
{
    *this = *this / A;
    return *this;
}

template <typename T>
T&
vec3<T>::operator[](size_t Index)
{
    if (Index >= 3)
    {
        ASSERT(!"Index Out of Bounds");
    }

    return this->E[Index];
}

template <typename T>
T
vec3<T>::SqMagnitude() const
{
    T Result = this->x*this->x + this->y*this->y + this->z*this->z;
    return Result;
}

template <typename T>
T
vec3<T>::Magnitude() const
{
    // NOTE: Change this to use ouw own Square Root function
    T Result = sqrtf(this->x*this->x + this->y*this->y + this->z*this->z);
    return Result;
}

template <typename T>
vec3<T>
vec3<T>::One()
{
    vec3<T> Result;

    Result.x = (T)1.0;
    Result.y = (T)1.0;
    Result.z = (T)1.0;

    return Result;
}

template <typename T>
T
SqMagnitude(const vec3<T> &A)
{
    T Result = A.x*A.x + A.y*A.y + A.z*A.z;
    return Result;
}

template <typename T>
T
Magnitude(const vec3<T> &A)
{
    T Result = sqrtf(A.x*A.x + A.y*A.y + A.z*A.z);
    return Result;
}

template <typename T>
vec3<T>
vec3<T>::Rand01()
{
    vec3<T> Result;
    Result.x = Rand01Generic<T>();
    Result.y = Rand01Generic<T>();
    Result.z = Rand01Generic<T>();
    return Result;
}

template <typename T>
vec3<T>
vec3<T>::RandRange(T Min, T Max)
{
    vec3<T> Result;
    Result.x = Min + ((Max - Min)*Rand01Generic<T>());
    Result.y = Min + ((Max - Min)*Rand01Generic<T>());
    Result.z = Min + ((Max - Min)*Rand01Generic<T>());
    return Result;
}

template <typename T>
vec3<T>
vec3<T>::RandomInUnitSphere()
{
    vec3<T> Result;
    while(1)
    {
        Result = vec3<T>::RandRange(-1, 1);
        // NOTE: Rejection Sampling. Reject all vectors that are outside the
        // unit sphere. Meaning they all have magnitudes more than the radius 1.
        if(Result.SqMagnitude() < (T)1)
        {
            break;
        }
    }

    return Result;
}

template <typename T>
inline vec3<T>
vec3<T>::RandomUnitVector()
{
    vec3<T> Result = RandomInUnitSphere();
    Result = Normalize(Result);

    return Result;
}

template <typename T>
inline vec3<T>
vec3<T>::RandomInUnitHemisphere(const vec3<T> &Normal)
{
    vec3<T> Result = RandomUnitVector();
    T DotP = DotStatic(Result, Normal);
    if(DotP < (T)0)
    {
        Result = -Result;
    }

    return Result;
}

template <typename T>
inline vec3<T>
vec3<T>::RandomInUnitDisk()
{
    vec3<T> Result;
    while(true)
    {
        Result.x = ::RandRange((T)-1, (T)1);
        Result.y = ::RandRange((T)-1, (T)1);
        Result.z = 0;

        if(Result.SqMagnitude() < 1.)
        {
            break;
        }
    }

    return Result;
}

template <typename T>
vec3<T>
Normalize(const vec3<T> &A)
{
    static_assert(std::is_same<T, f32>::value || std::is_same<T, f64>::value,
                  "Only f32 and f64 are supported");
    vec3<T> Result = A;
    T SqMagnitude = A.SqMagnitude();
    if(SqMagnitude > (T)0)
    {
        f64 OneByMagnitude = (T)1.0 / sqrt(SqMagnitude);
        Result *= OneByMagnitude;
    }
    return Result;
}

// ----------------------------------------------------------------------------------------------------------------
// Vec4
// ----------------------------------------------------------------------------------------------------------------
template <typename T>
vec4<T>
Vec4()
{
    vec4<T> Result = {(T)0, (T)0, (T)0, (T)0};
    return Result;
}

template <typename T>
vec4<T>
Vec4(T A)
{
    vec4<T> Result = {A, A, A, A};
    return Result;
}

template <typename T>
vec4<T>
Vec4(T x, T y, T z, T w)
{
    vec4<T> Result;
    Result.x = x;
    Result.y = y;
    Result.z = z;
    Result.w = w;
    return Result;
}

template <typename T>
vec4<T>
MakeVec4(const T *const Ptr)
{
    vec4<T> Result = {};
    memcpy(Result.E, Ptr, sizeof(T)*4);
    return Result;
}

template <typename T>
T
Dot(const vec4<T>& A, const vec4<T>& B)
{
    T Result = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;
    return Result;
}

template <typename T>
vec4<T>
operator+(const vec4<T>& A, const vec4<T>& B)
{
    vec4<T> Result = {A.x+B.x, A.y+B.y, A.z+B.z, A.w+B.w};
    return Result;
}

template <typename T>
vec4<T>
operator-(const vec4<T>& A, const vec4<T>& B)
{
    vec4<T> Result = {A.x-B.x, A.y-B.y, A.z-B.z, A.w-B.w};
    return Result;
}

template <typename T>
vec4<T>
operator*(const vec4<T>& A, const vec4<T>& B)
{
    vec4<T> Result = {A.x*B.x, A.y*B.y, A.z*B.z, A.w*B.w};
    return Result;
}

template <typename T>
vec4<T>
operator*(const vec4<T>& A, T B)
{
    vec4<T> Result = {A.x*B, A.y*B, A.z*B, A.w*B};
    return Result;
}

template <typename T>
vec4<T>
operator/(const vec4<T>& A, const vec4<T>& B)
{
    ASSERT((B.x != 0.0f) && (B.y != 0.0f) && (B.z != 0.0f) && (B.w != 0.0f));

    vec4<T> Result = {A.x/B.x, A.y/B.y, A.z/B.z, A.w/B.w};
    return Result;
}

template <typename T>
vec4<T>
operator/(const vec4<T>& A, T B)
{
    ASSERT(B != 0.0f);

    vec4<T> Result = {A.x/B, A.y/B, A.z/B, A.w/B};
    return Result;
}

template <typename T>
vec4<T>
vec4<T>::operator+=(const vec4<T>& A)
{
    *this = *this + A;
    return *this;
}

template <typename T>
vec4<T>
vec4<T>::operator-=(const vec4<T>& A)
{
    *this = *this - A;
    return *this;
}

template <typename T>
vec4<T>
vec4<T>::operator*=(T A)
{
    *this = *this * A;
    return *this;
}

template <typename T>
vec4<T>
vec4<T>::operator/=(const vec4<T>& A)
{
    *this = *this / A;
    return *this;
}

template <typename T>
vec4<T>
vec4<T>::operator/=(T A)
{
    *this = *this / A;
    return *this;
}

template <typename T>
T &
vec4<T>::operator[](size_t Index)
{
    if (Index >= 2)
    {
        ASSERT(!"Index Out of Bounds");
    }

    return this->E[Index];
}
#endif

#define VEC_H
#endif