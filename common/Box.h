#if !defined(BOX_H)

#include "defines.h"

#include "AARect.h"
#include "HittableList.h"

class box : public hittable
{
  public:
    box() {}
    box(const vec3d &P0, const vec3d &P1, std::shared_ptr<material> MaterialPtr);

    virtual b32 Hit(const ray &Ray, const interval &Interval,
                    hit_record &Record) const override;
    virtual b32
    BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const override
    {
        OutputBox = aabb(box_min, box_max);
        return true;
    }

  public:
    vec3d box_min;
    vec3d box_max;
    hittable_list sides;
};

box::box(const vec3d &P0, const vec3d &P1,
         std::shared_ptr<material> MaterialPtr)
{
    box_min = P0;
    box_max = P1;

    sides.Add(std::make_shared<xy_rect>(P0.x, P1.x, P0.y, P1.y, P1.z, MaterialPtr));
    sides.Add(std::make_shared<xy_rect>(P0.x, P1.x, P0.y, P1.y, P0.z, MaterialPtr));

    sides.Add(std::make_shared<xz_rect>(P0.x, P1.x, P0.z, P1.z, P1.y, MaterialPtr));
    sides.Add(std::make_shared<xz_rect>(P0.x, P1.x, P0.z, P1.z, P0.y, MaterialPtr));

    sides.Add(std::make_shared<yz_rect>(P0.y, P1.y, P0.z, P1.z, P1.x, MaterialPtr));
    sides.Add(std::make_shared<yz_rect>(P0.y, P1.y, P0.z, P1.z, P0.x, MaterialPtr));

}

b32
box::Hit(const ray &Ray, const interval &Interval, hit_record &Record) const
{
    b32 Result = sides.Hit(Ray, Interval, Record);
    return Result;
}

#define BOX_H
#endif