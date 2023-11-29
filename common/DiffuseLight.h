#if !defined(DIFFUSE_LIGHT_H)

#include "defines.h"
#include "Material.h"

class diffuse_light : public material
{
  public:
    diffuse_light(std::shared_ptr<texture> Tex) : emit(Tex) {}
    diffuse_light(color Color) 
    {
        std::shared_ptr<texture> ColTex = std::make_shared<solid_color>(Color);
        emit = ColTex;
    }

    virtual b32
    Scatter(const ray &RayIn, const hit_record &Record, color &Attenuation,
            ray &ScatteredRay) const override
    {
        return false;
    }
    
    virtual color
    Emitted(f64 U, f64 V, const vec3d &P) const override
    {
        color Result = emit->Value(U, V, P);
        return Result;
    }

  private:
    std::shared_ptr<texture> emit;
};  

#define DIFFUSE_LIGHT_H
#endif
