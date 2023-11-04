#if !defined(CAMERA_H)

#include "defines.h"
#include "Color.h"
#include "Hittable.h"
#include "File.h"

class camera
{
  public:
    f64 AspectRatio = 1.0;
    i32 ImageWidth = 100;
    const char *Filename;
    i32 NumSamples = 10; // Count of random samples around each pixel.
    
    void
    Render(const hittable &World)
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
                    PixelColor += RayColor(Ray, World);
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
    u8 *Data = nullptr;
    ppm PPMFile;
    
    b32 Initialized = false;
    
    void
    Initialize()
    {
        // Calculate Image Height and ensure that its atleast 1.
        this->ImageHeight = (i32)(this->ImageWidth / this->AspectRatio);
        this->ImageHeight = (this->ImageHeight < 1) ? 1 : this->ImageHeight;
        
        this->Center = Vec3d(0, 0, 0);
        
        // Calculate Viewport Dimensions
        f64 FocalLength = 1.0;
        f64 ViewportHeight = 2.0;
        f64 ViewportWidth = ViewportHeight * ((f64)(this->ImageWidth)/this->ImageHeight);
        
        // Calculate the vectors along the horizontal and down the vertical viewport
        // edges.
        vec3d ViewportU = Vec3d(ViewportWidth, 0, 0);
        vec3d ViewportV = Vec3d(0, -ViewportHeight, 0);
        
        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        this->PixelDeltaU = ViewportU / ImageWidth;
        this->PixelDeltaV = ViewportV / ImageHeight;
        
        // Calculate the location of the upper left pixel.
        vec3d ViewportUpperLeft = Center - Vec3d(0, 0, FocalLength) - ViewportU/2 - ViewportV/2;
        
        // Pixel Center of the upper left pixel. which is our origin
        this->Pixel00 = ViewportUpperLeft + 0.5*(this->PixelDeltaU + this->PixelDeltaV);
        
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
        vec3d PixelCenter = this->Pixel00 + (X*this->PixelDeltaU) + (Y*this->PixelDeltaV);
        
        // Random Position inside the Pixel Square
        vec3d PixelSample = PixelCenter + PixelSampleSquare();
        
        vec3d RayOrigin = this->Center;
        vec3d RayDirection = PixelSample - this->Center;
        
        ray Ray(RayOrigin, RayDirection);
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
    

    color
    RayColor(const ray &Ray, const hittable &World) const
    {
        // Render the "Hit" Object
        hit_record Record;
        if(World.Hit(Ray, interval(0, Infinity), Record))
        {
            // Converting Normal Vector Range -1,1 to 0,1 range so that it can be
            // used as a color.
            color Result = 0.5*(Record.Normal + vec3d::One());
            return Result;
        }
        
        // Render the Sky.
        vec3d UnitDirection = NormalizeDouble(Ray.Direction());
        // should be in the range (0,1) for color.
        f64 a = 0.5*(UnitDirection.y + 1.0);
        color Result = (1.0 - a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
        
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