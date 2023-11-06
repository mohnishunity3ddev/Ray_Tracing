#if !defined(MATERIAL_H)

#include "defines.h"
#include "Ray.h"
#include "Hittable.h"
#include "Color.h"
#include "Vec.h"

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
    metal(const color &a, const f64 &Fuzz)
        : albedo(a), fuzz((Fuzz < 1) ? Fuzz : 1)
    {
    }
    
    b32
    Scatter(const ray &RayIn, const hit_record &Record, color &Attenuation,
            ray &ScatteredRay) const override
    {
        vec3d InDir = Normalize(RayIn.Direction());
        vec3d ReflectedRay = Reflect(InDir, Record.Normal);

        vec3d FuzzVector = fuzz*(vec3d::RandomUnitVector());
        
        // The idea is that basically wherever the reflected ray ends up, we
        // make a sphere there with the radius = fuzz, and then the same
        // reflected ray gets a different endpoint on the surface of the sphere.
        // If the fuzz factor is more, then radius of that sphere will be more
        // which would increase the haziness/fuzziness of the reflections.
        ScatteredRay = ray(Record.P, ReflectedRay + FuzzVector);
        
        Attenuation = albedo;
        
        return true;
    }
  
  
  private:
    color albedo;

    // NOTE: To make the reflections fuzzy or a little bit hazy.
    // The more this fuzz factor, the more distorted/imperfect the reflected
    // vector is
    f64 fuzz; 
};

class dielectric : public material
{
  public:
    dielectric(f64 IndexOfRefraction) : indexOfRefraction(IndexOfRefraction) {}
    
    b32
    Scatter(const ray &RayIn, const hit_record &Record, color &Attenuation,
            ray &ScatteredRay) const override
    {
        Attenuation = Color(1., 1., 1.);
        f64 RefractionRatio = Record.FrontFace ? (1./indexOfRefraction) : indexOfRefraction;

        vec3d UnitDirection = Normalize(RayIn.Direction());
        f64 CosTheta = MIN(Dot(-UnitDirection, Record.Normal), 1.);
        f64 SinTheta = sqrt(1. - CosTheta*CosTheta);

        // NOTE: If this is > 1, then, this would invalidate snell's law.
        // SinTheta cannot be greater than 1.
        b32 TotalInternalReflection = (RefractionRatio*SinTheta > 1.);

        vec3d Direction;
        if(TotalInternalReflection || (Reflectance(CosTheta, RefractionRatio) > Rand01()))
        {
            Direction = Reflect(UnitDirection, Record.Normal);
        }
        else
        {
            Direction = Refract(UnitDirection, Record.Normal, RefractionRatio);
        }
        
        ScatteredRay = ray(Record.P, Direction);
        
        return true;
    }
  
  private:
    f64 indexOfRefraction;

    // NOTE: Every glass material has varied reflectance based on the angle of
    // incidence, how to get the reflectance is an ugly formula, but we can use
    // schlick approcimation here.
    f64
    Reflectance(f64 CosTheta, f64 RefractionRatio) const
    {
        f64 R0 = (1.0-RefractionRatio) / (1.0+RefractionRatio);
        R0 = R0*R0;
        f64 Result = R0 + (1.0-R0)*pow((1.0-CosTheta), 5);
        return Result;
    }
};

#define MATERIAL_H
#endif