#if !defined(CONSTANT_MEDIUM_H)

#include "defines.h"

#include "Hittable.h"
#include "Texture.h"
#include "Material.h"
#include "Vec.h"

// Such material scatters light uniformly in all directions.
class isotropic : public material
{
  public:
    isotropic(color c) : albedo(std::make_shared<solid_color>(c)) {}
    isotropic(std::shared_ptr<texture> a) : albedo(a) {}
    
    virtual b32
    Scatter(const ray &RayIn, const hit_record &Record, color &Attenuation,
            ray &ScatteredRay) const override
    {
        ScatteredRay = ray(Record.P, vec3d::RandomUnitVector(), RayIn.Time());
        Attenuation = albedo->Value(Record.U, Record.V, Record.P);
        return true;
    }

  public:
    std::shared_ptr<texture> albedo;
};

// A Volume with constant density
class constant_density_medium : public hittable
{
  public:
    constant_density_medium(std::shared_ptr<hittable> HittablePtr, f64 Density,
                            std::shared_ptr<texture> TexPtr)
        : boundary(HittablePtr), neg_inv_density(-1 / Density),
          phase_function(std::make_shared<isotropic>(TexPtr))
    {
    }

    constant_density_medium(std::shared_ptr<hittable> HittablePtr, f64 Density,
                            color Color)
        : boundary(HittablePtr), neg_inv_density(-1 / Density),
          phase_function(std::make_shared<isotropic>(Color))
    {
    }

    virtual b32 Hit(const ray &Ray, const interval &Interval,
                    hit_record &Record) const override;

    virtual b32
    BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const override
    {
        b32 Result = boundary->BoundingBox(Time0, Time1, OutputBox);

        return Result;
    }

  public:
    std::shared_ptr<hittable> boundary;
    std::shared_ptr<material> phase_function;
    f64 neg_inv_density;
};

b32
constant_density_medium::Hit(const ray &Ray, const interval &Interval,
                             hit_record &Record) const
{
    b32 Result = false;

    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool EnableDebug = false;
    const bool Debugging = EnableDebug && (Rand01() < 0.00001);

    hit_record Record1, Record2;

    if(boundary->Hit(Ray, interval(-Infinity, Infinity), Record1))
    {
        // Whether the ray hits the volume again with an increase in t
        if(boundary->Hit(Ray, interval(Record1.t+0.0001, Infinity), Record2))
        {
            // if (debugging)
            //     std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';

            if (Record1.t < Interval.Min) Record1.t = Interval.Min;
            if (Record2.t > Interval.Max) Record2.t = Interval.Max;

            // Check if the first intersection comes before the second.
            if (Record1.t < Record2.t)
            {
                if (Record1.t < 0) Record1.t = 0;

                const auto RayLength = Ray.Direction().Magnitude();
                // How much distance did the ray pass in.
                const auto DistanceInsideBoundary = (Record2.t - Record1.t)*RayLength;
                // A Random Hit Distance to check against the actual distance
                // the ray went in inside the volume.
                const auto RandomHitDistance = neg_inv_density*log(Rand01());

                // The RandomHitDistance Check to register the hit inside the volume.
                if (RandomHitDistance <= DistanceInsideBoundary)
                {
                    Record.t = Record1.t + RandomHitDistance / RayLength;
                    Record.P = Ray.At(Record.t);
                    /* if (debugging)
                    {
                        std::cerr << "hit_distance = " << hit_distance << '\n'
                                << "Record.t = " << Record.t << '\n'
                                << "Record.p = " << Record.p << '\n';
                    }
                    */
                    Record.Normal = Vec3d(1, 0, 0); // arbitrary
                    Record.FrontFace = true;      // also arbitrary
                    Record.Material = phase_function;

                    Result = true;
                }
            }
        }
    }

    return Result;
}

#define CONSTANT_MEDIUM_H
#endif