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

// IMPORTANT: NOTE: This class is for moving a hittable entity. By not actually
// moving it but moving the ray which is passed in the opposite of the required
// direction. This is similar to actually physically moving the hittable entity.
// Moving the ray in the opposite of the given direction is equivalent to moving
// the entity.
class translate : public hittable
{
  public:
    translate(std::shared_ptr<hittable> HittablePtr, const vec3d &Displacement)
        : hittablePtr(HittablePtr), offset(Displacement) {}

    virtual b32 Hit(const ray &Ray, const interval &Interval,
                    hit_record &Record) const override;

    virtual b32 BoundingBox(f64 Time0, f64 Time1,
                            aabb &OutputBox) const override;

  public:
    std::shared_ptr<hittable> hittablePtr;
    vec3d offset;
};

b32
translate::Hit(const ray &Ray, const interval &Interval, hit_record &Record) const
{
    b32 Result = false;
    ray MovedRay(Ray.Origin() - offset, Ray.Direction(), Ray.Time());

    if (hittablePtr->Hit(MovedRay, Interval, Record))
    {
        Record.P += offset;
        Record.SetFaceNormal(MovedRay, Record.Normal);
        Result = true;
    }

    return Result;
}

b32
translate::BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const
{
    b32 Result = false;
    if(hittablePtr->BoundingBox(Time0, Time1, OutputBox))
    {
        OutputBox = aabb(OutputBox.Min() + offset, OutputBox.Max() + offset);
        Result = true;
    }

    return Result;
}

class rotate_y : public hittable
{
  public:
    rotate_y(std::shared_ptr<hittable> P, f64 Angle);

    virtual b32 Hit(const ray &Ray, const interval &Interval,
                    hit_record &Record) const override;

    virtual b32 BoundingBox(f64 Time0, f64 Time1,
                            aabb &OutputBox) const override
    {
        OutputBox = bbox;

        b32 Result = hasBox;
        return Result;
    }

  public:
    std::shared_ptr<hittable> hittablePtr;
    f64 sin_theta;
    f64 cos_theta;
    b32 hasBox;
    aabb bbox;
};

rotate_y::rotate_y(std::shared_ptr<hittable> HittablePtr, f64 AngleInDegrees)
    : hittablePtr(HittablePtr)
{
    f64 Angle = Deg2Rad(AngleInDegrees);
    sin_theta = sin(Angle);
    cos_theta = cos(Angle);

    hasBox = hittablePtr->BoundingBox(0, 1, bbox);

    vec3d Min = Vec3d( Infinity,  Infinity,  Infinity);
    vec3d Max = Vec3d(-Infinity, -Infinity, -Infinity);

    // NOTE: Go through all the extreme points, rotate them and get the bounding
    // box of the rotated hittable.
    for(i32 i = 0; i < 2; ++i)
    {
        for(i32 j = 0; j < 2; ++j)
        {
            for(i32 k = 0; k < 2; ++k)
            {
                f64 x = i*bbox.Max().x + (1-i)*bbox.Min().x;
                f64 y = i*bbox.Max().y + (1-i)*bbox.Min().y;
                f64 z = i*bbox.Max().z + (1-i)*bbox.Min().z;

                // Rotate(prove this on paper on how to get the newX and newZ
                // when rotating around the Y Axis).
                f64 newX =  x*cos_theta + z*sin_theta;
                f64 newZ = -x*sin_theta + z*cos_theta;

                vec3d tester = Vec3d(newX, y, newZ);

                for(i32 c = 0; c < 3; ++c)
                {
                    Min[c] = MIN(Min[c], tester[c]);
                    Max[c] = MAX(Max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(Min, Max);
}

b32
rotate_y::Hit(const ray &Ray, const interval &Interval,
              hit_record &Record) const
{
    vec3d Origin = Ray.Origin();
    vec3d Direction = Ray.Direction();

    // NOTE: Rotate the ray in the opposite angle than which is given.
    Origin.x = cos_theta*Ray.Origin().x - sin_theta*Ray.Origin().z;
    Origin.z = sin_theta*Ray.Origin().x + cos_theta*Ray.Origin().z;
    Direction.x = cos_theta*Ray.Direction().x - sin_theta*Ray.Direction().z;
    Direction.z = sin_theta*Ray.Direction().x + cos_theta*Ray.Direction().z;

    ray RotatedRay = ray(Origin, Direction, Ray.Time());

    b32 Result = false;
    if(hittablePtr->Hit(RotatedRay, Interval, Record))
    {
        vec3d P = Record.P;
        vec3d Normal = Record.Normal;

        // NOTE: Actually Rotate the Position and Normal of the hit.
        P.x =  cos_theta*Record.P.x + sin_theta*Record.P.z;
        P.z = -sin_theta*Record.P.x + cos_theta*Record.P.z;

        Normal.x =  cos_theta*Record.Normal.x + sin_theta*Record.Normal.z;
        Normal.z = -sin_theta*Record.Normal.x + cos_theta*Record.Normal.z;

        Record.P = P;
        Record.SetFaceNormal(RotatedRay, Normal);

        Result = true;
    }

    return Result;
}

#define BOX_H
#endif