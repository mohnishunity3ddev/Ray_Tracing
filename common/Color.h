#if !defined(COLOR_H)

#include "Vec.h"

using color = vec3d;
color Color(f64 A, f64 B, f64 C)
{
    color Result = {A, B, C};
    return Result;
}

void
WriteColor(u8 **Data, const vec3d PixelColor)
{
    vec3u Col = {(u8)(255.999 * PixelColor.r), 
                 (u8)(255.999 * PixelColor.g),
                 (u8)(255.999 * PixelColor.b)};

    u8 *d = *Data;
    *d++ = Col.r;
    *d++ = Col.g;
    *d++ = Col.b;
    *Data = d;
}

#define COLOR_H
#endif