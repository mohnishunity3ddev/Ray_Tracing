#if !defined(HITTABLE_LIST_H)
#include "defines.h"
#include "Hittable.h"

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
};

#define HITTABLE_LIST_H
#endif