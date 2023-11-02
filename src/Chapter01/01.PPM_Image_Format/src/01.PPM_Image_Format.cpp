#include <cstdio>
#include <defines.h>
#include <File.h>

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
        for (i32 X = 0; X < Width; ++X)
        {
            double R = double(X) / (Width - 1);
            double G = double(Y) / (Height - 1);
            double B = 0;
            
            u8 iR = (u8)(255.999 * R);
            u8 iG = (u8)(255.999 * G);
            u8 iB = (u8)(255.999 * B);
            
            *Data++ = iR;
            *Data++ = iG;
            *Data++ = iB;
        }
    }
    
    WritePPM(&PPM);
    auto Result = ReadFile("OutputAnImage.ppm");
    
    free(PPM.ColorData);
    return 0;
}
