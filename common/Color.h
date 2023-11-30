#if !defined(COLOR_H)

#include "Vec.h"

using color = vec3d;
color Color(f64 A, f64 B, f64 C)
{
    color Result = {A, B, C};
    return Result;
}

f64
LinearSpaceToGamma(f64 LinearColorComponent)
{
    // Transformation is 1/gamma. Gamma is usually 2.2 but here, we are using an
    // approximation and using gamma 2. power of 1/2 is basically a sqrt
    f64 Result = sqrt(LinearColorComponent);
    return Result;
}

void
WriteColor(u8 **Data, const vec3d PixelColor, i32 NumSamples)
{
    f64 R = PixelColor.r;
    f64 G = PixelColor.g;
    f64 B = PixelColor.b;

    // Average out the color for a single pixel by dividing by the number of
    // samples
    f64 Scale = 1.0 / (f64)NumSamples;
    R *= Scale;
    G *= Scale;
    B *= Scale;

    // Transform to SRGB/Gamma Space
    R = LinearSpaceToGamma(R);
    G = LinearSpaceToGamma(G);
    B = LinearSpaceToGamma(B);

    static const interval IntensityInterval(0.0, 0.999);
    vec3u Col = {(u8)(256.*IntensityInterval.Clamp(R)),
                 (u8)(256.*IntensityInterval.Clamp(G)),
                 (u8)(256.*IntensityInterval.Clamp(B))};

    u8 *d = *Data;
    *d++ = Col.r;
    *d++ = Col.g;
    *d++ = Col.b;
    *Data = d;
}

#define COLOR_H
#endif