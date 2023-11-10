#if !defined(SPHERE_H)
#include "defines.h"
#include "Hittable.h"
#include <cmath>

class sphere : public hittable
{
  public:
    sphere(vec3d Center, f64 Radius, std::shared_ptr<material> Material)
        : center(Center), radius(Radius), mat(Material)
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
    Hit(const ray &Ray, const interval &Interval, hit_record &Record) const override
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
        if(!Interval.Surrounds(Root))
        {
            Root = (-Half_b + SqRootDiscriminant) / a;
            if(!Interval.Surrounds(Root))
            {
                return false;
            }
        }
        
        // The Root is actually a value of t that satisfies the ray-sphere
        // intersection quadratic eq.
        Record.t = Root;
        Record.P = Ray.At(Record.t);
        
        // This is a Unit Vector.
        vec3d OutwardNormal = ((Record.P - center) / radius);
        Record.set_face_normal(Ray, OutwardNormal);

        // NOTE: Update the UV Texture Coordinates.
        GetSphereUV(OutwardNormal, Record .U, Record.V);
        Record.Material = mat;
        
        return true;
    }
    
    b32
    BoundingBox(f64 Time0, f64 Time1, aabb &OutputBox) const override
    {
        OutputBox = aabb(center - Vec3d(radius), 
                         center + Vec3d(radius));
        return true;
    }
  
  private:
    vec3d center;
    f64 radius;
    std::shared_ptr<material> mat;
    
    static void
    GetSphereUV(const vec3d &P, f64 &U, f64 &V)
    {
        // NOTE: This Angle returned should be measured from -Y Axis. Therefore,
        // a point like (0, -1, 0) should return Theta as 0.
        f64 Theta = acos(-P.y);
        
        // IMPORTANT: NOTE:
        // In a sphere, if P = (-1, 0, 0) then u = 0
        // In a sphere, if P = ( 1, 0, 0) then u = 0.5
        // Because we are measuring the angle from -X axis(azimuthal angle in a
        // sphere). the entire u coordinate as it foes from 0-1 wraps the sphere
        // as the azimuthal angle goes from 0 to 2*pi as measured from -X.
        //
        // atan2 returns the angle 0,pi in the first and second coordinate in
        // the basis system where X is going right and y is going up. and it
        // measures the angle from the +X Axis. So a point like (-1, 0, 1) will
        // be in the Second Quadrant as far as atan2 is concerned and return
        // 0.75pi We want it to return 0.25pi according to our requirements
        // here.
        f64 Phi = atan2(-P.z, P.x) + pi;
        
        U = Phi / (2 * pi);
        V = Theta / pi;
    }
};

#define SPHERE_H
#endif