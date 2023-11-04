#if !defined(HITTABLE_H)
#include "defines.h"
#include "Ray.h"

class hit_record
{
  public:
    // Intersection point on the surface where the ray hit
    vec3d P;
    vec3d Normal;
    // the t in ray's eq: A + tB
    f64 t;
    b32 FrontFace;

    // NOTE: Sets the hit record normal vector
    // The OutwardNormal is assumed to be of unit length
    void set_face_normal(const ray &Ray, const vec3d &OutwardNormal)
    {
        // if the ray direction and the outward pointing normal passed in are
        // opposing vectors, then that means the ray is coming from outside the
        // hit surface, otherwise it is incident on the surface from the inside
        // of the surface.
        FrontFace = (Dot(Ray.Direction(), OutwardNormal) < 0.);
        Normal = FrontFace ? OutwardNormal : -OutwardNormal;
    }
};

class hittable
{
  public:
    virtual ~hittable() = default;
    virtual bool Hit(const ray &Ray, f64 Ray_TMin, f64 Ray_TMax, hit_record &Record) const = 0;
};

#define HITTABLE_H
#endif