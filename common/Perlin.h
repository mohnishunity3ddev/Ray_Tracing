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
        f64 U = P.x - floor(P.x);
        f64 V = P.y - floor(P.y);
        f64 W = P.z - floor(P.z);
        
        i32 I = (i32)(floor(P.x));
        i32 J = (i32)(floor(P.y));
        i32 K = (i32)(floor(P.z));
        
        f64 C[2][2][2];
        for(i32 dI = 0; dI < 2; ++dI)
        {
            for(i32 dJ = 0; dJ < 2; ++dJ)
            {
                for(i32 dK = 0; dK < 2; ++dK)
                {
                    i32 pX = permX[(I+dI) & 0xFF];
                    i32 pY = permY[(J+dJ) & 0xFF];
                    i32 pZ = permZ[(K+dK) & 0xFF];
                    C[dI][dJ][dK] = randFloat[pX ^ pY ^ pZ];
                }
            }
        }
        
        f64 Result = TrilinearInterp(C, U, V, W);
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
    
    static f64
    TrilinearInterp(f64 C[2][2][2], f64 U, f64 V, f64 W)
    {
        f64 Accum = 0.0;
        for(i32 i = 0; i < 2; ++i)
        {
            for(i32 j = 0; j < 2; ++j)
            {
                for(i32 k = 0; k < 2; ++k)
                {
                    f64 p1 = (i*U + (1-i)*(1-U));
                    f64 p2 = (j*V + (1-j)*(1-V));
                    f64 p3 = (k*W + (1-k)*(1-W));
                    Accum += p1*p2*p3*C[i][j][k];
                }
            }
        }
        
        return Accum;
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