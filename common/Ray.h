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
    vec3d origin() const { return this->orig; }
    vec3d direction() const { return dir; }
    vec3d at(double t) const 
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