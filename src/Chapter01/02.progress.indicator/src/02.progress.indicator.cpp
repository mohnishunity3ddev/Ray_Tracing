#include <cstdio>
#include <defines.h>
#include <File.h>
#include <Color.h>

#define INVERT_SCANLINES 0

int
main()
{
    // Image
    i32 Width = 256;
    i32 Height = 256;
    
    u64 RequiredSize = sizeof(u8)*Height*Width*3;
    u8 *Data = (u8 *)malloc(RequiredSize);
    memset(Data, 0, RequiredSize);
    
    ppm PPM = {};
    PPM.Filename = "OutputAnImage.ppm";
    PPM.Width = Width;
    PPM.Height = Height;
    PPM.ColorData = (u8 *)Data;
    PPM.Size = RequiredSize;
    
    for (i32 Y = 0; Y < Height; ++Y)
    {
        fprintf(stderr, "\rScanlines Remaining: %d ", (Height - Y));
        fflush(stderr);
        
        for (i32 X = 0; X < Width; ++X)
        {
            double R = double(X) / double(Width-1);
#if INVERT_SCANLINES
            double G = double(Height-Y-1) / double(Height-1);
#else
            double G = double(Y) / double(Height-1);
#endif
            double B = 0;
            
            vec3d Color = {R, G, B};
            WriteColor(&Data, Color);
        }
    }
    
    WritePPM(&PPM);
    auto Result = ReadFile("OutputAnImage.ppm");
    
    free(PPM.ColorData);
    return 0;
}
