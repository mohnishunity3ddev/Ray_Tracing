#include <cstdio>
#include <defines.h>

#include <HittableList.h>
#include <Sphere.h>
#include <Camera.h>

int
main()
{
    // World.
    hittable_list World;

    f64 R = cos(0.25*pi);

    auto LeftMaterial = std::make_shared<lambertian>(Color(0, 0, 1));
    auto RightMaterial = std::make_shared<lambertian>(Color(1, 0, 0));
    World.Add(std::make_shared<sphere>(Vec3d(-R, 0, -1), R, LeftMaterial));
    World.Add(std::make_shared<sphere>(Vec3d( R, 0, -1), R, RightMaterial));
    
    camera Camera;
    Camera.AspectRatio = 16.0/9.0;
    Camera.ImageWidth = 400;
    Camera.Filename = "RayTracer.ppm";
    Camera.NumSamples = 100;
    Camera.MaxBounces = 50;
    Camera.VerticalFOV = 45;

#if 0
    while(1)
    {
        vec3f V = vec3f::RandomOnHemisphere(vec3f::One());
        f64 Magnitude = V.Magnitude();
        f64 Dot = V.x + V.y + V.z;
        int x = 0;
    }
#endif
    
        
    Camera.Render(World);
    return 0;
}
