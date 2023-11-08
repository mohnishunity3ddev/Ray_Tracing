#if !defined(HITTABLE_LIST_H)
#include "defines.h"
#include "Hittable.h"
#include "AABB.h"

#include <memory>
#include <vector>

class hittable_list : public hittable
{
  public:
    std::vector<std::shared_ptr<hittable>> Objects;
    
    hittable_list() {}
    hittable_list(std::shared_ptr<hittable> Object) { Add(Object); }
    
    void Clear() { Objects.clear(); }
    
    void Add(std::shared_ptr<hittable> Object)
    {
        Objects.push_back(Object);
    }
    
    b32
    Hit(const ray &Ray, const interval &Interval, hit_record &Record) const override
    {
        hit_record TempRecord;
        b32 HitAnything = false;
        f64 ClosestSoFar = Interval.Max;
        
        for(const auto &Object : Objects)
        {
            if(Object->Hit(Ray, interval(Interval.Min, ClosestSoFar), TempRecord))
            {
                HitAnything = true; 
                ClosestSoFar = TempRecord.t;
                Record = TempRecord;
            }
        }
        
        return HitAnything;
    }
    
    b32
    BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const override
    {
        b32 Result = true;
        if(Objects.empty()) 
        {
            Result = false;
        }
        else
        {
            aabb TempBox;
            b32 FirstBox = true;
            
            for(const auto &Object : Objects)
            {
                if(!Object->BoundingBox(Time0, Time1, TempBox))
                {
                    Result = false;
                    break;
                }

                TempBox = FirstBox ? TempBox
                                   : aabb::SurroundingBox(TempBox, OutputBox);
                FirstBox = false;
            }
        }
        
        return Result;
    }
};

#define HITTABLE_LIST_H
#endif