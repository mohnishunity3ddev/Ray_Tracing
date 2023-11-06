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

    auto material_ground = std::make_shared<lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(Color(0.7, 0.3, 0.3));
    auto material_left = std::make_shared<metal>(Color(0.8, 0.8, 0.8));
    auto material_right = std::make_shared<metal>(Color(0.8, 0.6, 0.2));
    
    World.Add(make_shared<sphere>(Vec3d( 0.0, -100.5, -1.0), 100.0, material_ground));
    World.Add(make_shared<sphere>(Vec3d( 0.0,    0.0, -1.0),   0.5, material_center));
    World.Add(make_shared<sphere>(Vec3d(-1.0,    0.0, -1.0),   0.5, material_left));
    World.Add(make_shared<sphere>(Vec3d( 1.0,    0.0, -1.0),   0.5, material_right));    
    
    camera Camera;
    Camera.AspectRatio = 16.0/9.0;
    Camera.ImageWidth = 1920;
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
    
        
    Camera.Render(World);
    return 0;
}
