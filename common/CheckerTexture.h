#if !defined(CHECKER_TEXTURE_H)

#include "defines.h"
#include "Texture.h"

class checker_texture : public texture
{
  public:
    checker_texture() {}
    checker_texture(std::shared_ptr<texture> Even, std::shared_ptr<texture> Odd)
        : even(Even), odd(Odd) {}

    checker_texture(color C1, color C2)
        : even(std::make_shared<solid_color>(C1)),
          odd(std::make_shared<solid_color>(C2)) {}

    color
    Value(f64 U, f64 V, const vec3d &P) const override
    {
        f64 Sines = sin(10*P.x)*sin(10*P.y)*sin(10*P.z);
        color Result;
        if (Sines < 0.)
        {
            Result = this->odd->Value(U, V, P);
        }
        else
        {
            Result = this->even->Value(U, V, P);
        }

        return Result;
    }

  private:
    std::shared_ptr<texture> odd;
    std::shared_ptr<texture> even;
};

#define CHECKER_TEXTURE_H
#endif