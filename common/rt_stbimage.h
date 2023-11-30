#if !defined(RT_STB_IMAGE_H)
// Disable strict warnings for this header from the Microsoft Visual C++
// compiler.
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif

#include "defines.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#include <cstdlib>
#include <cstdio>

class rt_image
{
  public:
    rt_image() : Data(nullptr) {}
    rt_image(const char *Filename) { Load(Filename); }

    ~rt_image()
    {
        if(Data != nullptr)
        {
            STBI_FREE(Data);
            Data = nullptr;
        }
    }

    b32
    Load(const char *Filename)
    {
        auto N = BytesPerPixel;
        Data = stbi_load(Filename, &ImageWidth, &ImageHeight, &N, BytesPerPixel);
        BytesPerScanline = ImageWidth * BytesPerPixel;

        b32 Result = (Data != nullptr);
        return Result;
    }

    i32
    Width() const
    {
        i32 Result = Data == nullptr ? 0 : ImageWidth;
        return Result;
    }

    i32
    Height() const
    {
        i32 Result = Data == nullptr ? 0 : ImageHeight;
        return Result;
    }

    const u8 *
    PixelData(i32 X, i32 Y) const
    {
        u8 *Result = nullptr;

        // Error Pixel (Pink/Magenta)
        static u8 Magenta[] = { 255, 0, 255 };
        if(Data != nullptr)
        {
            X = Clamp(X, 0, ImageWidth);
            Y = Clamp(Y, 0, ImageHeight);
            Result = Data + Y*BytesPerScanline + X*BytesPerPixel;
        }
        else
        {
            Result = Magenta;
        }

        return Result;
    }

  private:
    const i32 BytesPerPixel = 3;
    u8 *Data;
    i32 ImageWidth, ImageHeight;
    i32 BytesPerScanline;

    internal i32
    Clamp(i32 X, i32 Low, i32 High)
    {
        // Return the value clamped to the range [low, high)
        i32 Result = X;
        if(X < Low) { Result = Low; }
        else if(X > High) { Result = High - 1; }

        return Result;
    }
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#define RT_STB_IMAGE_H
#endif