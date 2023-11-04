#if !defined(SPHERE_H)
#include "defines.h"
#include "Hittable.h"
#include <cmath>

class sphere : public hittable
{
  public:
    sphere(vec3d Center, f64 Radius) : center(Center), radius(Radius) {}
    
    // Returns true if the generated ray hits the sphere.
    // (x−Cx)^2 + (y−Cy)^2 + (z−Cz)^2 = r^2
    // (P−C)⋅(P−C) = (x−Cx)^2 + (y−Cy)^2 + (z−Cz)^2 [P is any point on sphere, C
    // is the center] (P−C)⋅(P−C) = r^2 [where r is the radius] If a ray
    // intersects the sphere on its surface, then,  (P(t)−C)⋅(P(t)−C) = r^2
    // ((A+tB)−C)⋅((A+tB)−C) = r^2 [(A + tB) is the ray. A is the Origin, B is
    // its direction] (B⋅B)*t^2 + 2*(B⋅(A−C))*t + ((A−C)⋅(A−C)−r^2)=0 [b here is
    // the ray direction, A is the ray origin] This is a quadratic equation,
    // Roots for t depends on the value of determinant .
    bool
    Hit(const ray &Ray, f64 Ray_TMin, f64 Ray_TMax,
        hit_record &Record) const override
    {
        vec3d OC = Ray.Origin() - center;
        
        // (B⋅B) is square magnitude of the vector.
        // a here is the coefficient of t^2 in the above quadratic equation.
        f64 a = Ray.Direction().SqMagnitude();
        // B⋅(A−C), b here is the coeffiecient of t in the quadratic equation.
        f64 Half_b = Dot(OC, Ray.Direction());
        // c here is the constant in the quadratic equation of t described
        // above. c = ((A−C)⋅(A−C)−r^2)
        f64 c = OC.SqMagnitude() - radius*radius;
        
        // NOTE:
        // -b -sqrt(b2 - 4ac) / 2a
        // if h = 2*b, this becomes -h - sqrt(h2 - ac) / a
        f64 Discriminant = Half_b*Half_b - a*c;
        // Meaning there are no real roots to the quadratic equation. Meaning
        // the ray does not HIT the sphere here.
        if(Discriminant < 0.)
        {
            return false;
        }
        
        f64 SqRootDiscriminant = sqrt(Discriminant);

        // Find the nearest root that lies within the acceptable range.
        f64 Root = (-Half_b - SqRootDiscriminant) / a;
        if(Root <= Ray_TMin || Root > Ray_TMax)
        {
            Root = (-Half_b + SqRootDiscriminant) / a;
            if(Root <= Ray_TMin || Root > Ray_TMax)
            {
                return false;
            }
        }

        // The Root is actually a value of t that satisfies the ray-sphere
        // intersection quadratic eq.
        Record.t = Root;
        Record.P = Ray.At(Record.t);
        Record.Normal = (Record.P - center) / radius;
        
        return true;
    }
  
  
  private:
    vec3d center;
    f64 radius;
};

#define SPHERE_H
#endif