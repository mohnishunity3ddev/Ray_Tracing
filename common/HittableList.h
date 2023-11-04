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
    hittable_list(std::shared_ptr<hittable> Object) { add(Object); }
    
    void clear() { Objects.clear(); }
    
    void add(std::shared_ptr<hittable> Object)
    {
        Objects.push_back(Object);
    }
    
    bool
    Hit(const ray &Ray, f64 Ray_TMin, f64 Ray_TMax,
        hit_record &Record) const override
    {
        hit_record TempRecord;
        b32 HitAnything = false;
        f64 ClosestSoFar = Ray_TMax;
        
        for(const auto &Object : Objects)
        {
            if(Object->Hit(Ray, Ray_TMin, ClosestSoFar, TempRecord))
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