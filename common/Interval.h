#if !defined(INTERVAL_H)

#include "defines.h"
class interval
{
  public:
    f64 Min, Max;
    interval() : Min(-Infinity), Max(+Infinity){}
    interval(f64 _Min, f64 _Max) : Min(_Min), Max(_Max) {}
    
    b32 Contains(f64 X) const
    {
        b32 Result = ((X >= Min) && (X <= Max));
        return Result;
    }
    
    b32 Surrounds(f64 X) const
    {
        b32 Result = ((X > Min) && (X < Max));
        return Result;
    }
    
    f64 Clamp(f64 X) const
    {
        f64 Result = X;
        if(X < Min)
        {
            Result = Min;
        }
        else if(X > Max)
        {
            Result = Max;
        }
        
        return Result;
    }
    
    static const interval empty, universe;
};

const static interval empty(+Infinity, -Infinity);
const static interval universe(-Infinity, +Infinity);

#define INTERVAL_H
#endif