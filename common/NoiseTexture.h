#if !defined(NOISE_TEXTURE_H)

#include "defines.h"
#include "Perlin.h"
#include "Texture.h"

class noise_texture : public texture
{
  public:
    noise_texture() : noise(perlin()) {}
    noise_texture(f64 Frequency) : frequency(Frequency), noise(perlin()) {}
    
    color
    Value(f64 U, f64 V, const vec3d &P) const override
    {
        color Result = Color(1, 1, 1)*noise.Noise(frequency*P);
        return Result;
    }
    
    perlin PerlinNoise() { return noise; }
  
  private:
    perlin noise;
    f64 frequency;
};

#define NOISE_TEXTURE_H
#endif