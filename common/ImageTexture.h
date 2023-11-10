#if !defined(IMAGE_TEXTURE_H)

#include "defines.h"
#include "Texture.h"
#include "rt_stbimage.h"
#include "Color.h"
#include "Interval.h"

class image_texture : public texture
{
  public:
    image_texture(const char *Filename) : image(Filename) {}
    
    color
    Value(f64 U, f64 V, const vec3d &P) const override
    {
        color Result;
        
        if(image.Height() <= 0)
        {
            // NOTE: If we have no image then return cyan as a debugging aid.
            Result = Color(0, 1, 1);
        }
        else
        {
            U = interval(0, 1).Clamp(U);
            // NOTE: Flip V to image coordinates.
            V = 1.0 - interval(0, 1).Clamp(V);
            
            i32 I = (i32)(U*image.Width());
            i32 J = (i32)(V*image.Height());
            
            const u8 *Pixel = image.PixelData(I, J);
            
            f64 ColorScale = 1.0 / 255.;
            
            Result = Color(Pixel[0]*ColorScale, 
                           Pixel[1]*ColorScale,
                           Pixel[2]*ColorScale);
        }
            
        return Result;
    }
  
  private:
    rt_image image;
};

#define IMAGE_TEXTURE_H
#endif