#if !defined(RAY_H)
#include "defines.h"
#include "Vec.h"

struct ray
{
  public:
    ray() {}
    ray(const vec3d &Origin, const vec3d &Direction) 
    {
        orig = Origin;
        dir = Direction;
    }
    inline vec3d Origin() const { return this->orig; }
    inline vec3d Direction() const { return dir; }
    inline vec3d At(double t) const 
    {
        vec3d Result = orig + dir*t;
        return Result;
    }
  
  private:
    vec3d orig;
    vec3d dir;
};

#define RAY_H
#endif