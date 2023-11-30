#if !defined(AARECT_H)

#include "defines.h"
#include "Hittable.h"

// NOTE: The XY Plane
class xy_rect : public hittable
{
  public:
    xy_rect() {}
    xy_rect(f64 X0, f64 X1, f64 Y0, f64 Y1, f64 _k, std::shared_ptr<material> Mat)
        : x0(X0), y0(Y0), x1(X1), y1(Y1), k(_k), mp(Mat) {}

    virtual b32 Hit(const ray &Ray, const interval &Interval,
                    hit_record &Record) const override;

    virtual b32
    BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const override
    {
        // NOTE: The bounding box must have non-zero width in each dimension, so
        // pad the Z with a small amount. This is needed for our BVH code to
        // work correctly.
        vec3d MinRange = Vec3d(x0, y0, k-0.0001);
        vec3d MaxRange = Vec3d(x1, y1, k+0.0001);

        OutputBox = aabb(MinRange, MaxRange);

        return true;
    }

  private:
    std::shared_ptr<material> mp;
    f64 x0, y0;
    f64 x1, y1;
    // This is the Z pos for this rectangle.
    f64 k;
};

// NOTE: The XZ Plane
class xz_rect : public hittable
{
  public:
    xz_rect() {}
    xz_rect(f64 X0, f64 X1, f64 Z0, f64 Z1, f64 _k,
            std::shared_ptr<material> Mat)
        : x0(X0), x1(X1), z0(Z0), z1(Z1), k(_k), mp(Mat)
    {
    }

    virtual b32 Hit(const ray &Ray, const interval &Interval,
                    hit_record &Record) const override;

    virtual b32
    BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const override
    {
        // NOTE: The bounding box must have non-zero width in each dimension, so
        // pad the Z with a small amount. This is needed for our BVH code to
        // work correctly.
        vec3d MinRange = Vec3d(x0, k-0.0001, z0);
        vec3d MaxRange = Vec3d(x1, k+0.0001, z1);

        OutputBox = aabb(MinRange, MaxRange);

        return true;
    }

  private:
    std::shared_ptr<material> mp;
    f64 x0, z0, x1, z1, k;
};

// NOTE: The YZ Plane
class yz_rect : public hittable
{
  public:
    yz_rect() {}
    yz_rect(f64 Y0, f64 Y1, f64 Z0, f64 Z1, f64 _k,
            std::shared_ptr<material> Mat)
        : y0(Y0), y1(Y1), z0(Z0), z1(Z1), k(_k), mp(Mat)
    {
    }

    virtual b32 Hit(const ray &Ray, const interval &Interval,
                    hit_record &Record) const override;

    virtual b32
    BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const override
    {
        // NOTE: The bounding box must have non-zero width in each dimension, so
        // pad the Z with a small amount. This is needed for our BVH code to
        // work correctly.
        vec3d MinRange = Vec3d(k-0.0001, y0, z0);
        vec3d MaxRange = Vec3d(k+0.0001, y1, z1);

        OutputBox = aabb(MinRange, MaxRange);

        return true;
    }

  private:
    std::shared_ptr<material> mp;
    f64 y0, z0, y1, z1, k;
};

b32 xy_rect::Hit(const ray &Ray, const interval &Interval,
                hit_record &Record) const
{
    b32 Result = false;

    // NOTE: k here is the rectangle's Z Position.
    f64 t = (k - Ray.Origin().z) / Ray.Direction().z;
    if ((t > Interval.Min) && (t < Interval.Max))
    {
        f64 x = Ray.Origin().x + t * Ray.Direction().x;
        f64 y = Ray.Origin().y + t * Ray.Direction().y;

        // NOTE: If the ray actually hits the insides of the rectangle.
        if ((x > x0) && (x < x1) && (y > y0) && (y < y1))
        {
            Record.U = (x - x0) / (x1 - x0);
            Record.V = (y - y0) / (y1 - y0);
            Record.t = t;

            vec3d OutwardNormal = Vec3d(0, 0, 1);
            Record.SetFaceNormal(Ray, OutwardNormal);
            Record.Material = mp;
            Record.P = Ray.At(t);
            Result = true;
        }
    }

    return Result;
}

b32 xz_rect::Hit(const ray &Ray, const interval &Interval,
                hit_record &Record) const
{
    b32 Result = false;

    // NOTE: k here is the rectangle's Z Position.
    f64 t = (k - Ray.Origin().y) / Ray.Direction().y;
    if ((t > Interval.Min) && (t < Interval.Max))
    {
        f64 x = Ray.Origin().x + t*Ray.Direction().x;
        f64 z = Ray.Origin().z + t*Ray.Direction().z;

        // NOTE: If the ray actually hits the insides of the rectangle.
        if ((x > x0) && (x < x1) && (z > z0) && (z < z1))
        {
            Record.U = (x-x0) / (x1-x0);
            Record.V = (z-z0) / (z1-z0);
            Record.t = t;

            vec3d OutwardNormal = Vec3d(0, 1, 0);
            Record.SetFaceNormal(Ray, OutwardNormal);
            Record.Material = mp;
            Record.P = Ray.At(t);
            Result = true;
        }
    }

    return Result;
}

b32 yz_rect::Hit(const ray &Ray, const interval &Interval,
                hit_record &Record) const
{
    b32 Result = false;

    // NOTE: k here is the rectangle's Z Position.
    f64 t = (k - Ray.Origin().x) / Ray.Direction().x;
    if ((t > Interval.Min) && (t < Interval.Max))
    {
        f64 y = Ray.Origin().y + t*Ray.Direction().y;
        f64 z = Ray.Origin().z + t*Ray.Direction().z;

        // NOTE: If the ray actually hits the insides of the rectangle.
        if ((y > y0) && (y < y1) && (z > z0) && (z < z1))
        {
            Record.U = (y-y0) / (y1-y0);
            Record.V = (z-z0) / (z1-z0);
            Record.t = t;

            vec3d OutwardNormal = Vec3d(1, 0, 0);
            Record.SetFaceNormal(Ray, OutwardNormal);
            Record.Material = mp;
            Record.P = Ray.At(t);
            Result = true;
        }
    }

    return Result;
}

#define AARECT_H
#endif