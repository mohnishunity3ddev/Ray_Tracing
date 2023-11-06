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

    auto Groundmaterial = std::make_shared<lambertian>(Color(0.8, 0.8, 0.0));
    auto Centermaterial = std::make_shared<lambertian>(Color(0.1, 0.2, 0.5));
    auto Leftmaterial   = std::make_shared<dielectric>(1.5);
    auto Rightmaterial  = std::make_shared<metal>(Color(0.8, 0.6, 0.2), 0.0);
    
    World.Add(std::make_shared<sphere>(Vec3d( 0.0, -100.5, -1.0), 100.0, Groundmaterial));
    World.Add(std::make_shared<sphere>(Vec3d( 0.0,    0.0, -1.0),   0.5, Centermaterial));
    World.Add(std::make_shared<sphere>(Vec3d(-1.0,    0.0, -1.0),   0.5, Leftmaterial));
    World.Add(std::make_shared<sphere>(Vec3d(-1.0,    0.0, -1.0),  -0.4, Leftmaterial));
    World.Add(std::make_shared<sphere>(Vec3d( 1.0,    0.0, -1.0),   0.5, Rightmaterial));
    
    camera Camera;
    Camera.AspectRatio = 16.0/9.0;
    Camera.ImageWidth = 400;
    Camera.Filename = "RayTracer.ppm";
    Camera.NumSamples = 100;
    Camera.MaxBounces = 50;
    
    // NOTE: Decrease this FOV Vertical to zoom in.
    Camera.VerticalFOV = 20;
    // Positioning the Camera.
    Camera.LookFrom = Vec3d(-2,  2,  1);
    // Orienting the Camera.
    Camera.LookAt   = Vec3d( 0,  0, -1);
    // The Global UP Vector used for calculating the camera's basis vectors.
    Camera.WorldUp  = Vec3d( 0,  1,  0);
    
    Camera.Render(World);
    return 0;
}
