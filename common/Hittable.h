#if !defined(HITTABLE_H)
#include "defines.h"
#include "Ray.h"

struct hit_record
{
    vec3d P;
    vec3d Normal;
    f64 t;
};

class hittable
{
  public:
    virtual ~hittable() = default;
    virtual bool Hit(const ray &Ray, f64 Ray_TMin, f64 Ray_TMax, hit_record &Record) const = 0;
};

#define HITTABLE_H
#endif