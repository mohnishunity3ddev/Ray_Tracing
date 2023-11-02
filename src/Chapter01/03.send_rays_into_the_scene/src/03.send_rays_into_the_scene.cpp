#include <Color.h>
#include <File.h>
#include <Ray.h>

#include <cstdio>
#include <defines.h>

#define INVERT_SCANLINES 0

color
GetRayColor(const ray &Ray)
{
    vec3d UnitDirection = NormalizeDouble(Ray.Direction());
    f64 a = 0.5*(UnitDirection.y + 1.0);

    color Result = (1.0 - a)*Color(1.0, 1.0, 1.0) + (a)*Color(0.5, 0.7, 1.0);
    return Result;
}

int
main()
{
    // Image
    f64 AspectRatio = 16.0 / 9.0;
    i32 ImageWidth = 400;

    // Calculate Image Height and ensure that its atleast 1.
    i32 ImageHeight = (i32)(ImageWidth / AspectRatio);
    ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;
    
    // Camera
    f64 FocalLength = 1.0;
    f64 ViewportHeight = 2.0;
    f64 ViewportWidth = ViewportHeight * ((f64)(ImageWidth) / ImageHeight);
    vec3d CameraCenter = Vec3d(0, 0, 0);

    // Calculate the vectors along the horizontal and down the vertical viewport edges.
    vec3d ViewportU = Vec3d(ViewportWidth, 0, 0);
    vec3d ViewportV = Vec3d(0, -ViewportHeight, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    vec3d PixelDeltaU = ViewportU / ImageWidth;
    vec3d PixelDeltaV = ViewportV / ImageHeight;

    // Calculate the location of the upper left pixel.
    vec3d ViewportUpperLeft = CameraCenter - Vec3d(0, 0, FocalLength) - ViewportU/2 - ViewportV/2;
    // Pixel Center of the upper left pixel.
    vec3d Pixel00 = ViewportUpperLeft + 0.5*(PixelDeltaU + PixelDeltaV);
    
    u64 RequiredSize = sizeof(u8) * ImageHeight * ImageWidth * 3;
    u8 *Data = (u8 *)malloc(RequiredSize);
    memset(Data, 0, RequiredSize);
    
    ppm PPM = {};
    PPM.Filename = "SendRaysIntoTheScene.ppm";
    PPM.Width = ImageWidth;
    PPM.Height = ImageHeight;
    PPM.ColorData = (u8 *)Data;
    PPM.Size = RequiredSize;
    
    for (i32 Y = 0; Y < ImageHeight; ++Y)
    {
        fprintf(stderr, "\rScanlines Remaining: %d ", (ImageHeight - Y));
        fflush(stderr);
        
        for (i32 X = 0; X < ImageWidth; ++X)
        {
            vec3d PixelCenter = Pixel00 + (X*PixelDeltaU) + (Y*PixelDeltaV);
            vec3d RayDirection = PixelCenter - CameraCenter;
            ray Ray(CameraCenter, RayDirection);

            color PixelColor = GetRayColor(Ray);
            WriteColor(&Data, PixelColor);
        }
    }
    
    WritePPM(&PPM);

    free(PPM.ColorData);
    return 0;
}
