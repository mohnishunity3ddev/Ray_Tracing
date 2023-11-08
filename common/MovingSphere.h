#if !defined(MOVING_SPHERE_H)

#include "defines.h"
#include "Vec.h"
#include "Hittable.h"

class moving_sphere : public hittable
{
  public:
    moving_sphere() {}
    moving_sphere(vec3d cen0, vec3d cen1, f64 t0, f64 t1, f64 r,
                  std::shared_ptr<material> matPtr)
        : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r),
          materialPtr(matPtr)
    {
    }
    
    // Returns true if the generated ray hits the sphere.
    // (x−Cx)^2 + (y−Cy)^2 + (z−Cz)^2 = r^2
    // (P−C)⋅(P−C) = (x−Cx)^2 + (y−Cy)^2 + (z−Cz)^2 [P is any point on sphere, C
    // is the center] (P−C)⋅(P−C) = r^2 [where r is the radius] If a ray
    // intersects the sphere on its surface, then,  (P(t)−C)⋅(P(t)−C) = r^2
    // ((A+tB)−C)⋅((A+tB)−C) = r^2 [(A + tB) is the ray. A is the Origin, B is
    // its direction] (B⋅B)*t^2 + 2*(B⋅(A−C))*t + ((A−C)⋅(A−C)−r^2)=0 [b here is
    // the ray direction, A is the ray origin] This is a quadratic equation,
    // Roots for t depends on the value of determinant .
    b32
    Hit(const ray &Ray, const interval &Interval,
        hit_record &Record) const override
    {
        vec3d SpherePosAtTime = Center(Ray.Time());
        vec3d OC = Ray.Origin() - SpherePosAtTime;
        
        // (B⋅B) is square magnitude of the vector.
        // a here is the coefficient of t^2 in the above quadratic equation.
        f64 a = Ray.Direction().SqMagnitude();
        // B⋅(A−C), b here is the coeffiecient of t in the quadratic equation.
        f64 Half_b = Dot(OC, Ray.Direction());
        // c here is the constant in the quadratic equation of t described
        // above. c = ((A−C)⋅(A−C)−r^2)
        f64 c = OC.SqMagnitude() - radius * radius;
        
        // NOTE:
        // -b -sqrt(b2 - 4ac) / 2a
        // if h = 2*b, this becomes -h - sqrt(h2 - ac) / a
        f64 Discriminant = Half_b * Half_b - a * c;
        // Meaning there are no real roots to the quadratic equation. Meaning
        // the ray does not HIT the sphere here.
        if (Discriminant < 0.)
        {
            return false;
        }
        
        f64 SqRootDiscriminant = sqrt(Discriminant);
        
        // Find the nearest root that lies within the acceptable range.
        f64 Root = (-Half_b - SqRootDiscriminant) / a;
        if (!Interval.Surrounds(Root))
        {
            Root = (-Half_b + SqRootDiscriminant) / a;
            if (!Interval.Surrounds(Root))
            {
                return false;
            }
        }
        
        // The Root is actually a value of t that satisfies the ray-sphere
        // intersection quadratic eq.
        Record.t = Root;
        Record.P = Ray.At(Record.t);
        Record.Material = materialPtr;
        
        // This is a Unit Vector.
        vec3d OutwardNormal = ((Record.P-SpherePosAtTime) / radius);
        Record.set_face_normal(Ray, OutwardNormal);
        
        return true;
    }

    b32
    BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const override
    {
        aabb Box0 = aabb(Center(Time0) - Vec3d(radius), 
                         Center(Time0) + Vec3d(radius));
        aabb Box1 = aabb(Center(Time1) - Vec3d(radius), 
                         Center(Time1) + Vec3d(radius));

        OutputBox = aabb::SurroundingBox(Box0, Box1);
        return true;
    }
    
    // NOTE: Where is the sphere located at the given time.
    vec3d
    Center(f64 Time) const
    {
        vec3d Result = center0 + ((Time-time0) / (time1-time0))*(center1-center0);
        return Result;
    }
  
  private:
    // The sphere is moving. It is at center0 at time t0 and center1 at time t1
    vec3d center0, center1;
    f64 time0, time1;
    f64 radius;
    std::shared_ptr<material> materialPtr;
};

#define MOVING_SPHERE_H
#endif