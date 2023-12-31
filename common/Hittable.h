#if !defined(HITTABLE_H)
#include "defines.h"
#include "Ray.h"
#include "Interval.h"
#include "AABB.h"

struct material;

class hit_record
{
  public:
    // Intersection point on the surface where the ray hit
    vec3d P;
    vec3d Normal;
    std::shared_ptr<material> Material; // The material of the hit object.
    f64 t; // the t in ray's eq: A + tB
    f64 U, V; // U and V surface coordinates of the ray-object hit point.
    b32 FrontFace;

    // NOTE: Sets the hit record normal vector
    // The OutwardNormal is assumed to be of unit length
    void SetFaceNormal(const ray &Ray, const vec3d &OutwardNormal)
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
    virtual b32 Hit(const ray &Ray, const interval &Interval,
                    hit_record &Record) const = 0;
    virtual b32 BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const = 0;
};

#define HITTABLE_H
#endif