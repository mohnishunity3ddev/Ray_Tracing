#if !defined(PERLIN_H)

#include "defines.h"
#include "Vec.h"

class perlin
{
  public:
    perlin() 
    {
        randFloat = new f64[pointCount];
        for(i32 Index = 0; Index < pointCount; ++Index)
        {
            randFloat[Index] = Rand01();
        }
        
        permX = PerlinGeneratePerm();
        permY = PerlinGeneratePerm();
        permZ = PerlinGeneratePerm();
    }
    
    ~perlin()
    {
        delete[] randFloat;
        delete[] permX;
        delete[] permY;
        delete[] permZ;
    }
    
    f64
    Noise(const vec3d &P) const
    {
        auto i = (i32)(4*P.x) & 0xFF;
        auto j = (i32)(4*P.y) & 0xFF;
        auto k = (i32)(4*P.z) & 0xFF;

        // NOTE: ^ is XOR operation. if two bits are different then the result is 1
        f64 Result = randFloat[permX[i] ^ permY[j] ^ permZ[k]];
        return Result;
    }
  
  private:
    static const i32 pointCount = 256;
    f64 *randFloat;
    i32 *permX, *permY, *permZ;
    
    static i32 *
    PerlinGeneratePerm()
    {
        auto P = new i32[pointCount];
        for(i32 Index = 0; Index < pointCount; ++Index)
        {
            P[Index] = Index;
        }
        
        Permute(P, pointCount);
        return P;
    }

    static void
    Permute(i32 *P, i32 N)
    {
        for(i32 Index = N-1; Index > 0; --Index)
        {
            i32 Target = RandomRangeInt(0, Index);
            i32 Tmp = P[Index];
            P[Index] = P[Target];
            P[Target] = Tmp;
        }
    }
};

#define PERLIN_H
#endif