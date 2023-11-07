#if !defined(RAY_H)
#include "defines.h"
#include "Vec.h"

struct ray
{
  public:
    ray() {}
    ray(const vec3d &Origin, const vec3d &Direction, f64 Time = 0.0) 
    {
        orig = Origin;
        dir = Direction;
        this->time = Time;
    }
    
    
    inline vec3d Origin() const { return this->orig; }
    inline vec3d Direction() const { return dir; }
    inline f64 Time() const { return time; }
    
    inline vec3d At(double t) const 
    {
        vec3d Result = orig + dir*t;
        return Result;
    }
    
  
  private:
    vec3d orig;
    vec3d dir;
    f64 time;
};

#define RAY_H
#endif