#if !defined(BVH_H)

#include "defines.h"
#include "Hittable.h"
#include "HittableList.h"

#include <algorithm>

class bvh_node : public hittable
{
  public:
    bvh_node() {}
    bvh_node(const hittable_list &List, f64 Time0, f64 Time1)
        : bvh_node(List.Objects, 0, List.Objects.size(), Time0, Time1)
    {
    }
    bvh_node(const std::vector<std::shared_ptr<hittable>> &SrcObjects,
             size_t Start, size_t End, f64 Time0, f64 Time1);
    
    virtual b32 Hit(const ray &Ray, const interval &Interval,
                    hit_record &Record) const override;
    
    virtual b32 BoundingBox(f64 Time0, f64 Time1,
                            aabb &OutputBox) const override;
  
  private:
    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb box;
};

inline b32
BoxCompare(const std::shared_ptr<hittable> A,
           const std::shared_ptr<hittable> B,
           i32 Axis)
{
    b32 Result = false;
    
    aabb BoxA;
    aabb BoxB;
    
    if(!A->BoundingBox(0, 0, BoxA) || 
       !B->BoundingBox(0, 0, BoxB))
    {
        printf("There was an error here in computing bounding boxes!\n");
        ASSERT(0);
        Result = false;
    }
    else
    {
        Result = (BoxA.Min()[Axis] < BoxB.Min()[Axis]);
    }
    
    return Result;
}

inline b32
BoxCompareXAxis(const std::shared_ptr<hittable> A,
            const std::shared_ptr<hittable> B)
{
    b32 Result = BoxCompare(A, B, 0);
    return Result;
}

inline b32
BoxCompareYAxis(const std::shared_ptr<hittable> A,
            const std::shared_ptr<hittable> B)
{
    b32 Result = BoxCompare(A, B, 1);
    return Result;
}

inline b32
BoxCompareZAxis(const std::shared_ptr<hittable> A,
            const std::shared_ptr<hittable> B)
{
    b32 Result = BoxCompare(A, B, 2);
    return Result;
}

// NOTE: Splitting BVH Volumes.
bvh_node::bvh_node(const std::vector<std::shared_ptr<hittable>> &SrcObjects,
                   size_t Start, size_t End, f64 Time0, f64 Time1)
{
    // Modifiable array of objects.
    auto tObjects = SrcObjects;
    
    //  Random Axis
    i32 RandomAxis = RandomRangeInt(0, 2);
    auto Comparator = (RandomAxis == 0) ? BoxCompareXAxis
                                        : ((RandomAxis == 1) ? BoxCompareYAxis
                                                             : BoxCompareZAxis);
    
    // NOTE: Splitting the BVH Volumes.
    size_t ObjectSpan = (End - Start);
    if(ObjectSpan == 1)
    {
        this->left = this->right = tObjects[Start];
    }
    else if(ObjectSpan == 2)
    {
        if(Comparator(tObjects[Start], tObjects[Start+1]))
        {
            this->left = tObjects[Start];
            this->right = tObjects[Start+1];
        }
        else
        {
            this->left = tObjects[Start+1];
            this->right = tObjects[Start];
        }
    }
    else
    {
        std::sort(tObjects.begin() + Start, tObjects.begin() + End, Comparator);
        
        auto Mid = Start + ObjectSpan/2;
        
        this->left = std::make_shared<bvh_node>(tObjects, Start, Mid, Time0, Time1);
        this->right = std::make_shared<bvh_node>(tObjects, Mid, End, Time0, Time1);
    }
    
    // NOTE: Set the bounding boxes of the child bvh nodes.
    aabb BoxLeft, BoxRight;
    if(!this->left->BoundingBox(Time0, Time1, BoxLeft) ||
        this->right->BoundingBox(Time0, Time1, BoxRight))
    {
        printf("No bounding box in bvh_node constructor.\n");
        ASSERT(0);
    }
    
    // NOTE: Parent Box contains both the child boxes.
    this->box = aabb::SurroundingBox(BoxLeft, BoxRight);
}

b32
bvh_node::BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const
{
    OutputBox = this->box;
    return true;
}

b32
bvh_node::Hit(const ray &Ray, const interval &Interval,
              hit_record &Record) const
{
    b32 Result = false;
    
    // Check whether the bounding box of this node is hit
    if(!this->box.Hit(Ray, Interval.Min, Interval.Max))
    {
        Result = false;
    }
    else
    {
        // NOTE: If the bounding box for this bvh is hit, check the child nodes
        // of this node to check whether they hit.
        hit_record LeftRecord, RightRecord;
        
        // These Left and Right Nodes can be bvh's or spheres or moving spheres.
        b32 HitLeft = this->left->Hit(Ray, Interval, LeftRecord);
        
        // NOTE: Both the child objects's bounding boxes can overlap
        b32 HitRight = this->right->Hit(Ray, Interval, RightRecord);
        
        // NOTE: If both children are hit by the ray
        if(HitLeft && HitRight)
        {
            // NOTE: We return the closer Object's surface point 't'
            if(LeftRecord.t < RightRecord.t)
            {
                Record = LeftRecord;
            }
            else
            {
                Record = RightRecord;
            }
            Result = true;
        }
        else if(HitLeft)
        {
            Record = LeftRecord;
            Result = true;
        }
        else if(HitRight)
        {
            Record = RightRecord;
            Result = true;
        }
    }
    
    return Result;
}

#define BVH_H
#endif