#if !defined(NOISE_TEXTURE_H)

#include "defines.h"
#include "Perlin.h"
#include "Texture.h"

class noise_texture : public texture
{
  public:
    noise_texture() : noise(perlin()) {}
    
    color
    Value(f64 U, f64 V, const vec3d &P) const override
    {
        color Result = Color(1, 1, 1)*noise.Noise(P);
        return Result;
    }
  
  private:
    perlin noise;
};

#define NOISE_TEXTURE_H
#endif