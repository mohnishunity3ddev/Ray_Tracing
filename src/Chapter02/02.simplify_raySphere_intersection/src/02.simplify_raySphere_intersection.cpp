#include <Color.h>
#include <File.h>
#include <Ray.h>

#include <cstdio>
#include <defines.h>

#define INVERT_SCANLINES 0

// Returns true if the generated ray hits the sphere.
// (x−Cx)^2 + (y−Cy)^2 + (z−Cz)^2 = r^2
// (P−C)⋅(P−C) = (x−Cx)^2 + (y−Cy)^2 + (z−Cz)^2 [P is any point on sphere, C is
// the center] (P−C)⋅(P−C) = r^2 [where r is the radius] If a ray intersects the
// sphere on its surface, then,  (P(t)−C)⋅(P(t)−C) = r^2
// ((A+tB)−C)⋅((A+tB)−C) = r^2 [(A + tB) is the ray. A is the Origin, B is its
// direction] (B⋅B)*t^2 + 2*(B⋅(A−C))*t + ((A−C)⋅(A−C)−r^2)=0 [b here is the ray
// direction, A is the ray origin] This is a quadratic equation, Roots for t
// depends on the value of determinant .
f64
HitSphere(const vec3d &SphereCenter, f64 SphereRadius, const ray &Ray)
{
    vec3d OC = Ray.Origin() - SphereCenter;
    
    // (B⋅B) is square magnitude of the vector.
    // a here is the coefficient of t^2 in the above quadratic equation.
    f64 a = Ray.Direction().SqMagnitude();
    // B⋅(A−C), b here is the coeffiecient of t in the quadratic equation.
    f64 Half_b = Dot(OC, Ray.Direction());
    // c here is the constant in the quadratic equation of t described above.
    // c = ((A−C)⋅(A−C)−r^2)
    f64 c = OC.SqMagnitude() - SphereRadius*SphereRadius;
    
    // NOTE:
    // -b -sqrt(b2 - 4ac) / 2a
    // if h = 2*b, this becomes -h - sqrt(h2 - ac) / a
    f64 Discriminant = Half_b*Half_b - a*c;
    
    f64 Result = 0.;
    if(Discriminant < 0.)
    {
        Result = -1.;
    }
    else
    {
        Result = (-Half_b - sqrt(Discriminant)) / (a);
    }
    
    return Result;
}

color
GetRayColor(const ray &Ray)
{
    vec3d SphereCenter = Vec3d(0, 0, -1);
    f64 t = HitSphere(SphereCenter, 0.5, Ray);
    // The Sphere is in front of the camera not behind.
    if (t > 0.0)
    {
        vec3d N = NormalizeDouble(Ray.At(t) - SphereCenter);
        // Normal in the range (0, 1)
        color NormalColor = 0.5 * Color(N.x + 1, N.y + 1, N.z + 1);
        return NormalColor;
    }
    
    vec3d UnitDirection = NormalizeDouble(Ray.Direction());
    // should be in the range 0,1 for color.
    f64 a = 0.5*(UnitDirection.y + 1.0);
    
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
    PPM.Filename = "SimplifyRaySphereIntersection.ppm";
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
            vec3d PixelCenter = Pixel00 + (X * PixelDeltaU) + (Y * PixelDeltaV);
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
