#if !defined(FILE_H)
#include "defines.h"
#include <cstdio>
#include <memory>

struct ppm
{
    const char *Filename;
    
    i32 Width;
    i32 Height;
    
    u8 *ColorData;
    u64 Size;
};

struct file_read_info
{
    void *Data;
    u64 Size;
};

void
WritePPM(const ppm *PPM)
{
    FILE *File = fopen(PPM->Filename, "wb");
    if (!File)
    {
        printf("There was an error opening file: %s\n", PPM->Filename);
        return;
    }
    
    fprintf(File, "P6\n%d %d\n255\n", PPM->Width, PPM->Height);
    
    u64 WriteElements = fwrite(PPM->ColorData, sizeof(u8), PPM->Size, File);
    
    fclose(File);
    File = nullptr;
}

file_read_info
ReadFile(const char *Filename)
{
    file_read_info Result = {};
    
    FILE *File = fopen(Filename, "rb");
    if (!File)
    {
        printf("There was an error opening file: %s\n", Filename);
        return Result;
    }
    
    fseek(File, 0, SEEK_END);
    u64 Size = ftell(File);
    fseek(File, 0, SEEK_SET);
    
    if (Size > 0)
    {
        u8 *Data = (u8 *)malloc(Size);
        
        if (Data)
        {
            u64 BytesRead = fread(Data, sizeof(u8), Size, File);
            if (BytesRead == Size)
            {
                Result.Data = Data;
                Result.Size = Size;
            }
            else
            {
                Result.Data = nullptr;
                Result.Size = 0;
                free(Data);
                Data = nullptr;
            }
        }
    }
    
    fclose(File);
    
    return Result;
}
#define FILE_H
#endif