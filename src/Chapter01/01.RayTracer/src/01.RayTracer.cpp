#include <cstdio>
#include <defines.h>

#include <HittableList.h>
#include <Sphere.h>
#include <Camera.h>

int
main()
{
    camera Camera;
    Camera.AspectRatio = 16.0/9.0;
    Camera.ImageWidth = 400;
    Camera.Filename = "RayTracer.ppm";
    Camera.NumSamples = 100;
    Camera.MaxBounces = 50;

#if 0
    while(1)
    {
        vec3f V = vec3f::RandomOnHemisphere(vec3f::One());
        f64 Magnitude = V.Magnitude();
        f64 Dot = V.x + V.y + V.z;
        int x = 0;
    }
#endif
    
    // World.
    hittable_list World;
    World.Add(std::make_shared<sphere>(Vec3d(0, 0, -1), 0.5));
    World.Add(std::make_shared<sphere>(Vec3d(0, -100.5, -1), 100.0));
    
    Camera.Render(World);
    
    return 0;
}
