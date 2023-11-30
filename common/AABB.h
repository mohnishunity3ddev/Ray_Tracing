#if !defined(AABB_H)

#include "defines.h"
#include "Ray.h"
#include "Interval.h"
#include "Vec.h"

class aabb
{
  public:
    aabb() {}
    aabb(const vec3d &a, const vec3d &b) : minimum(a), maximum(b) {}

    vec3d Min() const { return minimum; }
    vec3d Max() const { return maximum; }

    // NOTE: Ray-AABB Intersection
    // TMin, TMax are the tValues for which the ray is inside the 3D aabb
    // region.
    b32
    Hit(const ray &Ray, f64 TMin, f64 TMax) const
    {
        b32 Result = true;

        for(i32 Index = 0;
            Index < 3;
            ++Index)
        {
            f64 InvDir = 1.0 / Ray.Direction()[Index];

            f64 t0 = (Min()[Index] - Ray.Origin()[Index])*InvDir;
            f64 t1 = (Max()[Index] - Ray.Origin()[Index])*InvDir;
            if(InvDir < 0.0)
            {
                Swap(t0, t1);
            }

            TMin = MAX(t0, TMin);
            TMax = MIN(t1, TMax);

            if(TMax <= TMin)
            {
                Result = false;
                break;
            }
        }

        return Result;
    }

    // NOTE: The Bounding Box of two boxes.
    static aabb
    SurroundingBox(const aabb &Box0, const aabb &Box1)
    {
        vec3d Min = Vec3d(MIN(Box0.Min().x, Box1.Min().x),
                          MIN(Box0.Min().y, Box1.Min().y),
                          MIN(Box0.Min().z, Box1.Min().z));
        vec3d Max = Vec3d(MAX(Box0.Max().x, Box1.Max().x),
                          MAX(Box0.Max().y, Box1.Max().y),
                          MAX(Box0.Max().z, Box1.Max().z));

        aabb Result = aabb(Min, Max);
        return Result;
    }

  private:
    vec3d minimum;
    vec3d maximum;
};

#define AABB_H
#endif