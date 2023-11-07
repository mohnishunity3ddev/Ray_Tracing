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

    auto GroundMaterial = std::make_shared<lambertian>(Color(0.5, 0.5, 0.5));
    World.Add(std::make_shared<sphere>(Vec3d(0, -1000, 0), 1000, GroundMaterial));
    
    for (i32 X = -11; X < 11; X++)
    {
        for (i32 Y = -11; Y < 11; Y++)
        {
            auto ChooseMaterial = Rand01();
            vec3d Center = Vec3d(X + 0.9*Rand01(), 0.2, Y + 0.9*Rand01());
            
            if ((Center - Vec3d(4, 0.2, 0)).Magnitude() > 0.9)
            {
                std::shared_ptr<material> SphereMaterial;
                
                if (ChooseMaterial < 0.8)
                {
                    // diffuse
                    auto albedo = color::Rand01() * color::Rand01();
                    SphereMaterial = std::make_shared<lambertian>(albedo);
                    World.Add(std::make_shared<sphere>(Center, 0.2, SphereMaterial));
                }
                else if (ChooseMaterial < 0.95)
                {
                    // metal
                    auto albedo = color::RandRange(0.5, 1);
                    auto fuzz = RandRange(0, 0.5);
                    SphereMaterial = std::make_shared<metal>(albedo, fuzz);
                    World.Add(std::make_shared<sphere>(Center, 0.2, SphereMaterial));
                }
                else
                {
                    // glass
                    SphereMaterial = std::make_shared<dielectric>(1.5);
                    World.Add(std::make_shared<sphere>(Center, 0.2, SphereMaterial));
                }
            }
        }
    }
    
    auto material1 = std::make_shared<dielectric>(1.5);
    World.Add(std::make_shared<sphere>(Vec3d(0, 1, 0), 1.0, material1));
    
    auto material2 = std::make_shared<lambertian>(Vec3d(0.4, 0.2, 0.1));
    World.Add(std::make_shared<sphere>(Vec3d(-4, 1, 0), 1.0, material2));
    
    auto material3 = std::make_shared<metal>(Vec3d(0.7, 0.6, 0.5), 0.0);
    World.Add(std::make_shared<sphere>(Vec3d(4, 1, 0), 1.0, material3));
    
    camera Camera;
    Camera.AspectRatio = 16.0/9.0;
    Camera.ImageWidth = 1200;
    Camera.Filename = "FinalRender_RayTracingInAWeekend.ppm";
    Camera.NumSamples = 500;
    Camera.MaxBounces = 50;
    
    // NOTE: Decrease this FOV Vertical to zoom in.
    Camera.VerticalFOV = 20;
    // Positioning the Camera.
    Camera.LookFrom = Vec3d(13,  2,  3);
    // Orienting the Camera.
    Camera.LookAt   = Vec3d( 0,  0,  0);
    // The Global UP Vector used for calculating the camera's basis vectors.
    Camera.WorldUp  = Vec3d( 0,  1,  0);

#if 0
    vec3d FocusAt = Vec3d(0, 0, -1);
    f64 Dist = (FocusAt - Camera.LookFrom).Magnitude();
#endif

    // NOTE: Depth of Field Parameters
    // This is how we are handling depth of field.
    // When objects are far from the focal distance then rays emanating from the
    // camera after intersecting the focal plane diverge a lot(if the ray
    // origins are different as we are doing here using the defocus disk), the
    // divergence will be more for objects away from the focal plane hence
    // blurring will be more for those objects.
    
    // NOTE: Aperture basically. More this angle, more will be defocus blur(More
    // blurring on the objects which are not in focus).
    Camera.DefocusAngle = 0.6;
    // NOTE: This is where the camera is focused. Basically this object or
    // objects close to this distance will be focused on and hence wont be
    // blurred.
    Camera.FocusDistance = 10.0;
    
    Camera.Render(World);
    
    return 0;
}
