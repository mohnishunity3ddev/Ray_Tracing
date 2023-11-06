#if !defined(MATERIAL_H)

#include "defines.h"
#include "Ray.h"
#include "Hittable.h"
#include "Color.h"

// NOTE: Material class for different kinds of materials in the scene.
// It has two jobs:
// 1. Produce a scattered ray or absorb the ray(Based on the material's
// properties).
// 2. If scattered, how much the ray should be attenuated(Meaning how much of
// its "intensity" is reflected and the rest absorbed).
class material
{
  public:
    virtual ~material() = default;

    // NOTE: Produces a scattered ray based on the incident ray. This function
    // basically simulates how the incident ray gets reflected by the surface
    // with this kind of a material.
    virtual b32 Scatter(const ray &RayIn, const hit_record &Record, color &Attenuation, ray &ScatteredRay) const = 0;
};

class lambertian : public material
{
  public:
    lambertian(const color &A) : albedo(A) {}

    b32
    Scatter(const ray &RayIn, const hit_record &Record, color &Attenuation,
            ray &ScatteredRay) const override
    {
        // Lambertian Law which states that lambertian surfaces reflect light
        // much closer to thenormal of the surface point where the ray was
        // incident.
        vec3d ScatteredDirection = Record.Normal + vec3d::RandomUnitVector();
        
        // NOTE: RandomUnitVector could return the negative of the Record.Normal
        // in which case the ScatteredDirection will be Zero or NearZero. to
        // avoid divide by zero errors later along with some other unexpected
        // issues, we for this here and make sure its not zero.
        if(ScatteredDirection.NearZero())
        {
            ScatteredDirection = Record.Normal;
        }
        
        ScatteredRay = ray(Record.P, ScatteredDirection);
        Attenuation = albedo;
        return true;
    }
  
  private:
    color albedo;
};

// Metals are supposed to Reflect the incident ray not scatter it
class metal : public material
{
  public:
    metal(const color &a) : albedo(a) {}
    
    b32
    Scatter(const ray &RayIn, const hit_record &Record, color &Attenuation,
            ray &ScatteredRay) const override
    {
        vec3d InDir = Normalize(RayIn.Direction());
        vec3d ReflectedRay = Reflect(InDir, Record.Normal);

        ScatteredRay = ray(Record.P, ReflectedRay);
        Attenuation = albedo;

        return true;
    }
  
  
  private:
    color albedo;
};

#define MATERIAL_H
#endif