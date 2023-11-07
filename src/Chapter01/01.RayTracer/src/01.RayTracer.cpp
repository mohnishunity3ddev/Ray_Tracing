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
    
    vec3d FocusAt = Vec3d(0, 0, -1);
    f64 Dist = (FocusAt - Camera.LookFrom).Magnitude();

    // NOTE: Depth of Field Parameters
    // This is how we are handling depth of field.
    // When objects are far from the focal distance then rays emanating from the
    // camera after intersecting the focal plane diverge a lot(if the ray
    // origins are different as we are doing here using the defocus disk), the
    // divergence will be more for objects away from the focal plane hence
    // blurring will be more for those objects.

    // NOTE: Aperture basically. More this angle, more will be defocus blur(More
    // blurring on the objects which are not in focus).
    Camera.DefocusAngle = 0.0;
    // NOTE: This is where the camera is focused. Basically this object or
    // objects close to this distance will be focused on and hence wont be
    // blurred.
    Camera.FocusDistance = Dist;
    
    Camera.Render(World);

    printf("\n HitCount: %d\n", Camera.HitCount);
    return 0;
}
