#if !defined(CAMERA_H)

#include "defines.h"
#include "Color.h"
#include "Hittable.h"
#include "File.h"
#include "Material.h"

class camera
{
  public:
    i32 ImageWidth = 100;
    const char *Filename;
    i32 NumSamples = 10; // Count of random samples around each pixel.
    i32 MaxBounces = 10; // The Maximum number of bounces the rays are allowed to have.
    
    
    camera() {}
    camera(vec3d lookFrom, vec3d lookAt, vec3d globalUpVec, f64 vFov,
           i32 imageWidth, f64 aspectRatio, f64 defocusAngle, f64 DistToFocus,
           f64 shutterOpenTime, f64 shutterCloseTime)
        : LookFrom(lookFrom), LookAt(lookAt), WorldUp(globalUpVec),
          VerticalFOV(vFov), ImageWidth(imageWidth), AspectRatio(aspectRatio),
          DefocusAngle(defocusAngle), FocusDistance(DistToFocus),
          ShutterOpenTime(shutterOpenTime), ShutterCloseTime(shutterCloseTime) {}   
    
    void
    Render(const hittable &World, const color &Background)
    {
        if(!Initialized)
        {
            Initialize();
        }
        
        for(i32 Y = 0; Y < this->ImageHeight; ++Y)
        {
            fprintf(stderr, "\rScanlines Remaining: %d ", (ImageHeight - Y));
            fflush(stderr);
            
            for(i32 X = 0; X < this->ImageWidth; ++X)
            {
                color PixelColor = Color(0, 0, 0);
                
                // Take the required number of samples
                for(i32 SampleIndex = 0; 
                    SampleIndex < NumSamples; 
                    ++SampleIndex)
                {
                    // Basically sample around a random position inside the pixel "square"
                    ray Ray = GetRandomRayAround(X, Y);
                    PixelColor += RayColor(Ray, Background, MaxBounces, World);
                }
                    
                WriteColor(&this->Data, PixelColor, this->NumSamples);
            }
        }
        
        WritePPM(&this->PPMFile);
        FreeImageData();
    }
  
  private:
    i32 ImageHeight;
    vec3d Center;       // Camera Center
    vec3d Pixel00;      // Location of the 0,0 Pixel in the upper left.
    vec3d PixelDeltaU;  // Offset to pixel to the right.
    vec3d PixelDeltaV;  // Offset to pixel to the left.
    vec3d U, V, W;      // Camera Ortho-Normal Basis Vectors.
    vec3d DefocusDiskU;
    vec3d DefocusDiskV;
    
    f64 AspectRatio = 1.0;
    
    f64 VerticalFOV = 90.0;           // Vertical Field of View of the camera.
    vec3d LookFrom = Vec3d(0, 0, -1); // Where the camera is Looking From.
    vec3d LookAt = Vec3d(0, 0, 0);    // Where the camera is Looking At.
    vec3d WorldUp = Vec3d(0, 1, 0);   // The Global Up Vector.
    
    // NOTE: Depth of Field Parameters
    // This is how we are handling depth of field.
    f64 DefocusAngle = 0;
    f64 FocusDistance = 10;
    
    // NOTE: Motion Blur
    // A real world camera opens the shutter for a specificn period of time. It
    // takes in all the light in this interval and averages it out and so if the
    // objects are moving, we see some blur.
    f64 ShutterOpenTime = 0.;
    f64 ShutterCloseTime = 0.;
    
    u8 *Data = nullptr;
    ppm PPMFile;
    
    b32 Initialized = false;
    
    void
    Initialize()
    {
        // Calculate Image Height and ensure that its atleast 1.
        this->ImageHeight = (i32)(this->ImageWidth / this->AspectRatio);
        this->ImageHeight = (this->ImageHeight < 1) ? 1 : this->ImageHeight;
        
        this->Center = LookFrom;
        
        // Calculate Viewport Dimensions
        // FocalLength is distance between the camera center and the image plane.
        f64 VerticalAngle = Deg2Rad(this->VerticalFOV);
        f64 h = tan(VerticalAngle*0.5);
        
        f64 ViewportHeight = (2.0*h)*(this->FocusDistance);
        f64 ViewportWidth = ViewportHeight * ((f64)(this->ImageWidth)/this->ImageHeight);
        
        // Calculating the Camera Basis Vectors.
        this->W = Normalize(LookFrom - LookAt);
        this->U = Normalize(Cross(this->WorldUp, this->W));
        this->V = Cross(this->W, this->U);
        
        // Calculate the vectors along the horizontal and down the vertical viewport
        // edges.
        vec3d ViewportU =  ViewportWidth * this->U;
        vec3d ViewportV = -ViewportHeight * this->V;
        
        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        this->PixelDeltaU = ViewportU / this->ImageWidth;
        this->PixelDeltaV = ViewportV / this->ImageHeight;
        
        // NOTE: Calculate the location of the upper left pixel. Sets the image
        // plane at the focus distance also.
        vec3d ViewportUpperLeft = Center - (this->FocusDistance*this->W) - (ViewportU/2) - (ViewportV/2);
        
        // Pixel Center of the upper left pixel. which is our origin
        this->Pixel00 = ViewportUpperLeft + 0.5*(this->PixelDeltaU + this->PixelDeltaV);
        
        // Calculate the camera defocus disk basis vectors.
        f64 DefocusRadius = this->FocusDistance*tan(Deg2Rad(0.5*this->DefocusAngle));
        this->DefocusDiskU = this->U * DefocusRadius;
        this->DefocusDiskV = this->V * DefocusRadius;
        
        u64 RequiredSize = sizeof(u8)*this->ImageHeight*this->ImageWidth*3;
        this->Data = (u8 *)malloc(RequiredSize);
        memset(this->Data, 0, RequiredSize);
        
        PPMFile = {};
        PPMFile.Filename = this->Filename;
        PPMFile.Width = this->ImageWidth;
        PPMFile.Height = this->ImageHeight;
        PPMFile.ColorData = (u8 *)this->Data;
        PPMFile.Size = RequiredSize;
        
        Initialized = true;
    }
    
    ray
    GetRandomRayAround(i32 X, i32 Y) const
    {
        // NOTE: Get a randomly-sampled camera ray for the pixel at location
        // i,j, originating from the camera defocus disk.
        
        vec3d PixelCenter = this->Pixel00 + (X*this->PixelDeltaU) + (Y*this->PixelDeltaV);
        
        // Random Position inside the Pixel Square
        vec3d PixelSample = PixelCenter + PixelSampleSquare();
        
        vec3d RayOrigin = (this->DefocusAngle <= 0) ? this->Center : DefocusDiskSample();
        vec3d RayDirection = PixelSample - RayOrigin;
        
        // NOTE: The way we do motion blur, is that we select a single ray in
        // random times in the interval of the time when the shutter is open.
        f64 RayTime = RandRange(ShutterOpenTime, ShutterCloseTime);
        
        ray Ray = ray(RayOrigin, RayDirection, RayTime);
        return Ray;
    }
    
    vec3d PixelSampleSquare() const
    {
        // Random value b/w [-0.5,0.5)
        f64 X = -0.5 + Rand01();
        f64 Y = -0.5 + Rand01();
        
        // Random Position Around the Pixel Square
        vec3d Result = X*this->PixelDeltaU + Y*this->PixelDeltaV;
        
        return Result;
    }
    
    vec3d
    DefocusDiskSample() const
    {
        // NOTE: Returns a random point in the camera defocus disk.
        vec3d P = vec3d::RandomInUnitDisk();
        vec3d Result = this->Center + (P.x*this->DefocusDiskU + P.y*this->DefocusDiskV);
        
        return Result;
    }

    color
    RayColor(const ray &Ray, const color &Background, i32 BounceCount,
             const hittable &World)
    {
        // Render the "Hit" Object
        hit_record Record;
        color Result = Color(0, 0, 0);

        // Only Continue if the light ray has not crossed our max bounce
        // threshold.
        if(BounceCount > 0)
        {
            // NOTE:
            // There’s also a subtle bug that we need to address. A ray will attempt
            // to accurately calculate the intersection point when it intersects
            // with a surface. Unfortunately for us, this calculation is susceptible
            // to floating point rounding errors which can cause the intersection
            // point to be ever so slightly off. This means that the origin of the
            // next ray, the ray that is randomly scattered off of the surface, is
            // unlikely to be perfectly flush with the surface. It might be just
            // above the surface. It might be just below the surface. If the ray's
            // origin is just below the surface then it could intersect with that
            // surface again. Which means that it will find the nearest surface at
            // t=0.00000001 or whatever floating point approximation the hit
            // function gives us. The simplest hack to address this is just to
            // ignore hits that are very close to the calculated intersection point:
            f64 Correction = 0.001;
            interval HitInterval = interval(Correction, Infinity);
            
            if (!World.Hit(Ray, HitInterval, Record))
            {
                Result = Background;
            }
            else
            {
                ray Scattered;
                color Attenuation;
                color Emitted = Record.Material->Emitted(Record.U, Record.V, Record.P);
                
                if(!Record.Material->Scatter(Ray, Record, Attenuation, Scattered))
                {
                    Result = Emitted;
                }
                else
                {
                    Result = Emitted + (Attenuation*RayColor(Scattered, Background,
                                                             BounceCount-1, World));
                }
            }
        }

#if RENDER_SKY
        // NOTE: Render the Sky.
        vec3d UnitDirection = Normalize(Ray.Direction());
        // should be in the range (0,1) for color.
        f64 a = 0.5*(UnitDirection.y + 1.0);
        color Result = (1.0 - a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
#endif
        
        return Result;
    }
    
    void
    FreeImageData()
    {
        if(this->PPMFile.ColorData)
        {
            free(this->PPMFile.ColorData);
            this->Data = nullptr;
        }
        
        this->PPMFile = {};
    }
};

#define CAMERA_H
#endif