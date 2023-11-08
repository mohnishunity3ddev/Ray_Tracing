#if !defined(TEXTURE_H)

#include "defines.h"
#include "Color.h"
#include "Vec.h"

class texture
{
  public:
    virtual color Value(f64 U, f64 V, const vec3d &P) const = 0;
};

class solid_color : public texture
{
  public:
    solid_color() {}
    solid_color(color C) : color_value(C) {}
    solid_color(f64 Red, f64 Green, f64 Blue)
        : solid_color(Color(Red, Green, Blue)) {}
    
    virtual color Value(f64 U, f64 V, const vec3d &P) const override
    {
        color Result = color_value;
        return Result;
    }
  
  private:
    color color_value;
};

#define TEXTURE_H
#endif