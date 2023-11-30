#include <cstdio>
#include <defines.h>

#include <HittableList.h>
#include <Sphere.h>
#include <MovingSphere.h>
#include <Camera.h>
#include <CheckerTexture.h>
#include <Perlin.h>
#include <NoiseTexture.h>
#include <ImageTexture.h>
#include <DiffuseLight.h>
#include <AARect.h>
#include <Box.h>

hittable_list
RandomScene()
{
    // World.
    hittable_list Result;

    auto CheckerTex = std::make_shared<checker_texture>(Color(0.2, 0.3, 0.1),
                                                        Color(0.9, 0.9, 0.9));

    auto GroundMaterial = std::make_shared<lambertian>(CheckerTex);
    Result.Add(std::make_shared<sphere>(Vec3d(0, -1000, 0), 1000, GroundMaterial));

    for(i32 X = -11; X < 11; X++)
    {
        for(i32 Y = -11; Y < 11; Y++)
        {
            auto ChooseMaterial = Rand01();
            vec3d Center = Vec3d(X + 0.9*Rand01(), 0.2, Y + 0.9*Rand01());

            if((Center - Vec3d(4, 0.2, 0)).Magnitude() > 0.9)
            {
                std::shared_ptr<material> SphereMaterial;
                if (ChooseMaterial < 0.8)
                {
                    // diffuse
                    vec3d albedo = color::Rand01() * color::Rand01();
                    SphereMaterial = std::make_shared<lambertian>(albedo);

                    // Where the sphere goes at time t1, since it is moving.
                    vec3d RandomHalfY = Vec3d(0, RandRange(0, 0.5), 0);
                    vec3d Center1 = Center + RandomHalfY;
                    moving_sphere MovingSphere = moving_sphere(Center, Center1, 0, 1, 0.2, SphereMaterial);
                    Result.Add(std::make_shared<moving_sphere>(MovingSphere));
                }
                else if (ChooseMaterial < 0.95)
                {
                    // metal
                    vec3d albedo = color::RandRange(0.5, 1);
                    f64 fuzz = RandRange(0, 0.5);
                    SphereMaterial = std::make_shared<metal>(albedo, fuzz);
                    Result.Add(std::make_shared<sphere>(Center, 0.2, SphereMaterial));
                }
                else
                {
                    // glass
                    SphereMaterial = std::make_shared<dielectric>(1.5);
                    Result.Add(std::make_shared<sphere>(Center, 0.2, SphereMaterial));
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    Result.Add(std::make_shared<sphere>(Vec3d(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(Vec3d(0.4, 0.2, 0.1));
    Result.Add(std::make_shared<sphere>(Vec3d(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(Vec3d(0.7, 0.6, 0.5), 0.0);
    Result.Add(std::make_shared<sphere>(Vec3d(4, 1, 0), 1.0, material3));

    return Result;
}

hittable_list
TwoSpheres()
{
    hittable_list Result;

    auto CheckerTex = std::make_shared<checker_texture>(Color(0.2, 0.3, 0.1),
                                                        Color(0.9, 0.9, 0.9));

    Result.Add(std::make_shared<sphere>(Vec3d(0, -10, 0), 10, std::make_shared<lambertian>(CheckerTex)));
    Result.Add(std::make_shared<sphere>(Vec3d(0,  10, 0), 10, std::make_shared<lambertian>(CheckerTex)));

    return Result;
}

hittable_list
EarthScene()
{
    hittable_list Result;

    auto EarthTex = std::make_shared<image_texture>("../images/earthmap.jpg");
    auto EarthSurface = std::make_shared<lambertian>(EarthTex);
    auto Globe = std::make_shared<sphere>(Vec3d(0, 0, 0), 2, EarthSurface);

    Result = hittable_list(Globe);

    return Result;
}

hittable_list
TwoPerlinSpheres()
{
    hittable_list Result;

    auto PerlinTex = std::make_shared<noise_texture>(4.0);

    Result.Add(std::make_shared<sphere>(Vec3d(0, -1000, 0), 1000, std::make_shared<lambertian>(PerlinTex)));
    Result.Add(std::make_shared<sphere>(Vec3d(0, 2, 0), 2, std::make_shared<lambertian>(PerlinTex)));

    return Result;
}

hittable_list
SimpleLight()
{
    hittable_list Objects;

    std::shared_ptr<noise_texture> PerlinTex = std::make_shared<noise_texture>(4.);

    // IMPORTANT: NOTE:
    // In this scene, there is a simple diffuse light and two lambertian
    // material spheres. Light does not scatter light, it just emits light. In
    // the Camera.RayColor function which computes the color the ray will get
    // What it also does is based on how far a pixel is from the light.
    // How does it do it?
    // A pixel on the lambertian sphere reflects light(Scatters), if it scatters
    // light towards the light, then it gets the color of the light. More pixels
    // reflecting rays towards the light, More bright the surface pixel is due
    // to the said light. Hence, far off pixels are less bright due to the
    // light because less rays are getting scattered towards the light. If this
    // was Rasterization, then this is what Attenuation factor does by dividing
    // the color of the surface by the distance squared between the surface
    // pixel and the light.;
    Objects.Add(std::make_shared<sphere>(Vec3d(0, -1000, 0), 1000, std::make_shared<lambertian>(PerlinTex)));
    Objects.Add(std::make_shared<sphere>(Vec3d(0, 2, 0), 2, std::make_shared<lambertian>(PerlinTex)));


    std::shared_ptr<material> DiffLight = std::make_shared<diffuse_light>(Color(4, 4, 4));
    Objects.Add(std::make_shared<sphere>(Vec3d(0, 7, 0), 1, DiffLight));
    Objects.Add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, DiffLight));

    return Objects;
}

hittable_list
CornellBox()
{
    hittable_list Objects;

    auto RedMat = std::make_shared<lambertian>(Color(.65, .05, .05));
    auto GreenMat = std::make_shared<lambertian>(Color(.12, .45, .15));
    // auto BlueMat = std::make_shared<lambertian>(Color(.12, .15, .55));
    auto WhiteMat = std::make_shared<lambertian>(Color(.73, .73, .73));
    auto Light = std::make_shared<diffuse_light>(Color(15, 15, 15));

    Objects.Add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, GreenMat));  // Left Wall
    Objects.Add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, RedMat));      // Right wall
    Objects.Add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, Light)); // Light at the top
    Objects.Add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, WhiteMat));    // Bottom Wall
    Objects.Add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, WhiteMat));   // Top Wall
    Objects.Add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, WhiteMat));  // Front Wall


    // NOTE: Add two boxes inside the room
    Objects.Add(std::make_shared<box>(Vec3d(130, 0, 65), Vec3d(295, 165, 230), WhiteMat));
    Objects.Add(std::make_shared<box>(Vec3d(265, 0, 295), Vec3d(430, 330, 460), WhiteMat));

    return Objects;
}

int
main()
{
    hittable_list World;

    // NOTE: Camera Params
    // NOTE: Depth of Field Parameters
    // This is how we are handling depth of field.
    // When objects are far from the focal distance then rays emanating from the
    // camera after intersecting the focal plane diverge a lot(if the ray
    // origins are different as we are doing here using the defocus disk), the
    // divergence will be more for objects away from the focal plane hence
    // blurring will be more for those objects.
    // NOTE: Aperture basically. More this angle, more will be defocus blur(More
    // blurring on the objects which are not in focus).
    f64 DefocusAngle = 0.0;
    // NOTE: This is where the camera is focused. Basically this object or
    // objects close to this distance will be focused on and hence wont be
    // blurred.
    f64 FocusDistance = 10.0;
    // NOTE: Decrease this FOV Vertical to zoom in.
    f64 VerticalFOV = 40.0;
    // Positioning and Orienting the Camera.
    vec3d LookFrom, LookAt;
    // The Global UP Vector used for calculating the camera's basis vectors.
    vec3d Up = Vec3d(0, 1, 0);
    // NOTE: Motion Blur Parameters
    // This is the time interval when the shutter of the virtual camera is open.
    f64 ShutterOpenTime = 0.;
    f64 ShutterCloseTime = 1.;

    f64 AspectRatio = (16.0 / 9.0);

    i32 WorldSelect = 6;
    color Background = Color(0, 0, 0);

    i32 SamplesPerPixel = 100;
    i32 ImageWidth = 400;

    switch(WorldSelect)
    {
        case 1:
        {
            World = RandomScene();
            Background = Color(0.7, 0.8, 1.0);
            VerticalFOV = 20;
            LookFrom = Vec3d(13, 2, 3);
            LookAt = Vec3d(0, 0, 0);
            DefocusAngle = 0.6;
        } break;

        case 2:
        {
            World = TwoSpheres();
            Background = Color(0.7, 0.8, 1.0);
            VerticalFOV = 20;
            LookFrom = Vec3d(13, 2, 3);
            LookAt = Vec3d(0, 0, 0);
        } break;

        case 3:
        {
            World = EarthScene();
            Background = Color(0.7, 0.8, 1.0);
            VerticalFOV = 20;
            LookFrom = Vec3d(0, 0, 12);
            LookAt = Vec3d(0, 0, 0);
            DefocusAngle = 0;
        } break;

        case 4:
        {
            World = TwoPerlinSpheres();
            Background = Color(0.7, 0.8, 1.0);
            VerticalFOV = 20;
            LookFrom = Vec3d(13, 2, 3);
            LookAt = Vec3d(0, 0, 0);
        } break;

        case 5:
        {
            World = SimpleLight();
            SamplesPerPixel = 400;
            Background = Color(0, 0, 0);
            LookFrom = Vec3d(26, 3, 6);
            LookAt = Vec3d(0, 2, 0);
            VerticalFOV = 20.0;
        } break;

        case 6:
        {
            World = CornellBox();
            AspectRatio = 1.0;
            ImageWidth = 600;
            SamplesPerPixel = 200;
            Background = Color(0, 0, 0);
            LookFrom = Vec3d(278, 278, -800);
            LookAt = Vec3d(278, 278, 0);
            VerticalFOV = 40.0;
        } break;

        default: {}
    }

#if 0
    vec3d FocusAt = Vec3d(0, 0, -1);
    f64 Dist = (FocusAt - Camera.LookFrom).Magnitude();
#endif

    camera Cam = camera(LookFrom, LookAt, Up, VerticalFOV, ImageWidth, AspectRatio,
                        DefocusAngle, FocusDistance, ShutterOpenTime, ShutterCloseTime);
    Cam.Filename = "07a_Instances_Box.ppm";
    Cam.NumSamples = SamplesPerPixel;
    Cam.MaxBounces = 50;
    Cam.Render(World, Background);
    return 0;
}
