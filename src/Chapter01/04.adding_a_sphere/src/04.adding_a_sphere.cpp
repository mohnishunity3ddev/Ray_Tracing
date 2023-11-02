#include <Color.h>
#include <File.h>
#include <Ray.h>

#include <cstdio>
#include <defines.h>

#define INVERT_SCANLINES 0

// Returns true if the generated ray hits the sphere.
// (x−Cx)^2 + (y−Cy)^2 + (z−Cz)^2 = r^2
// (P−C)⋅(P−C) = (x−Cx)^2 + (y−Cy)^2 + (z−Cz)^2 [P is any point on sphere, C is the center]
// (P−C)⋅(P−C) = r^2 [where r is the radius]
// If a ray intersects the sphere on its surface, then,  (P(t)−C)⋅(P(t)−C) = r^2
// ((A+tb)−C)⋅((A+tb)−C) = r^2 [(A + tB) is the ray. A is the Origin, B is its direction]
// (b⋅b)*t^2 + 2*b⋅(A−C)*t + ((A−C)⋅(A−C)−r^2)=0 [b here is the ray direction, A is the ray origin]
// This is a quadratic equation, Roots for t depends on the value of determinant .
b32
HitSphere(const vec3d &Center, f64 Radius, const ray &Ray)
{
    vec3d RayOMinusSphereC = Ray.Origin() - Center;
    f64 a = Dot(Ray.Direction(), Ray.Direction());
    f64 b = 2.0*Dot(RayOMinusSphereC, Ray.Direction());
    f64 c = Dot(RayOMinusSphereC, RayOMinusSphereC) - Radius*Radius;
    
    // This is the determinant to the equations solving for ray-sphere
    // intersection.
    f64 Determinant = b*b - 4*a*c;
    
    b32 Result = false;
    if(Determinant >= 0)
    {
        f64 RootD = sqrt(Determinant);
        f64 Inv2a = 1.0 / (2*a);
        f64 T0 = (-b + RootD)*Inv2a;
        f64 T1 = (-b - RootD)*Inv2a;
        if(T0 > 0 || T1 > 0)
        {
            Result = true;
        }
    }
    
    return Result;
}

color
GetRayColor(const ray &Ray)
{
    if(HitSphere(Vec3d(0, 0, -1), 0.5, Ray))
    {
        return Color(1, 0, 0);
    }

    vec3d UnitDirection = NormalizeDouble(Ray.Direction());
    f64 a = 0.5 * (UnitDirection.y + 1.0);
    
    color Result = (1.0 - a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
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
    
    // Calculate the vectors along the horizontal and down the vertical viewport
    // edges.
    vec3d ViewportU = Vec3d(ViewportWidth, 0, 0);
    vec3d ViewportV = Vec3d(0, -ViewportHeight, 0);
    
    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    vec3d PixelDeltaU = ViewportU / ImageWidth;
    vec3d PixelDeltaV = ViewportV / ImageHeight;
    
    // Calculate the location of the upper left pixel.
    vec3d ViewportUpperLeft =
        CameraCenter - Vec3d(0, 0, FocalLength) - ViewportU / 2 - ViewportV / 2;
    // Pixel Center of the upper left pixel.
    vec3d Pixel00 = ViewportUpperLeft + 0.5 * (PixelDeltaU + PixelDeltaV);
    
    u64 RequiredSize = sizeof(u8) * ImageHeight * ImageWidth * 3;
    u8 *Data = (u8 *)malloc(RequiredSize);
    memset(Data, 0, RequiredSize);
    
    ppm PPM = {};
    PPM.Filename = "AddingASphere.ppm";
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
