#if !defined(MONTE_CARLO_H)

#include "defines.h"

#define FUNC(name) f64 name(f64 x)
typedef FUNC(DoubleInDoubleOut);

struct sample
{
    f64 x;
    f64 p_x;
};

class MC
{
  public:
    static void EstimatePi();
    static void RunningEstimatePi();
    // NOTE: Stratified Sampling(Jittering)
    static void StratifiedEstimatePi();
    static void OneDimensionalIntegration(DoubleInDoubleOut *IntegrandFunction,
                                          f64 LowerBound, f64 UpperBound,
                                          i64 NumSamples);
    static void ComputePDFHalfwayPoint(DoubleInDoubleOut *PDFFunction, f64 LowerBound, f64 UpperBound);
    static void ImportanceSampling();
    static void SurfaceIntegralOverSphere();

  private:
    static b32 CompareByX(const sample &a, const sample &b);
};

b32
MC::CompareByX(const sample &a, const sample &b)
{
    b32 Result = a.x < b.x;
    return Result;
}

void
MC::EstimatePi()
{
    i32 N = 1000'000;
    i32 InsideCircle = 0;

    // NOTE: Here we make N tries. Everytime we will get a point inside the
    // square whose x,y go from (-1, 1). Then we compute how many of these
    // points lie within a circle whose radius is 1 and its the incircle of
    // this square. All these if summed up can be seen as the area of the
    // circle and the sqaure. So InsideCircle / N = Area of circle / Area of
    // sqaure = pi*r*r / (2*r)^2 = pi / 4. So 4 * InsideCircle / N will give
    // us a value of pi.
    for (i32 Index = 0; Index < N; ++Index)
    {
        f64 x = RandRange(-1, 1);
        f64 y = RandRange(-1, 1);
        // Length of the vector given by the above x,y should be less than 1
        // to be inside the circle.
        if (x*x + y*y < 1)
        {
            ++InsideCircle;
        }
    }

    f64 pi = 4.0*((f64)InsideCircle / (f64)N);
    printf("The Pi Evaluated comes out to be %f\n", pi);
}

void
MC::RunningEstimatePi()
{
    i64 Runs = 0;
    i64 InsideCircle = 0;

    while(1)
    {
        ++Runs;

        f64 x = RandRange(-1, 1);
        f64 y = RandRange(-1, 1);
        if((x*x + y*y) < 1)
        {
            ++InsideCircle;
        }

        if((Runs % 1000'000) == 0)
        {
            f64 pi = 4.0*((f64)InsideCircle / (f64)Runs);
            printf("Estimate of pi in %lld runs = %f.\n", Runs, pi);
        }

    }
}

// Basically take a grid inside the -1,1 square. and take samples inside the
// individual grid squares instead of the whole square.
//
// IMPORTANT: NOTE:
// The reason why stratified is better is because it takes into
// account all the subregions inside the square depending on how much you have
// divided the square. So, it makes sure each subregion gets equal number of
// samples, you get a uniformity like that which means that you converge to an
// estimate of pi quickly.
void
MC::StratifiedEstimatePi()
{
    i32 InsideCircle = 0;
    i32 InsideCircleStratified = 0;
    i32 SqrtN = 1000;

    // Dividing the -1,1 square into a 1000 by 1000 grid.
    for(i32 i = 0; i < SqrtN; ++i)
    {
        for(i32 j = 0; j < SqrtN; ++j)
        {
            f64 x = RandRange(-1, 1);
            f64 y = RandRange(-1, 1);
            if((x*x + y*y) < 1)
            {
                ++InsideCircle;
            }

            // Getting a point inside an individual grid square.
            // The square goes from -1 to 1, that's why multiplying 2 and
            // subtracting to bring the value from (0,1) to (-1, 1).
            x = 2*((i + Rand01()) / SqrtN) - 1;
            y = 2*((j + Rand01()) / SqrtN) - 1;
            if((x*x + y*y) < 1)
            {
                ++InsideCircleStratified;
            }
        }
    }

    f64 RegularPI = 4.0*((f64)InsideCircle / (f64)(SqrtN*SqrtN));
    f64 StratifiedPI = 4.0*((f64)InsideCircleStratified / (f64)(SqrtN*SqrtN));
    printf("Regular Estimate of pi:%f and Stratified estimate of pi is: %f.\n",
           RegularPI, StratifiedPI);
}

void
MC::OneDimensionalIntegration(DoubleInDoubleOut *IntegrandFunction,
                              f64 LowerBound, f64 UpperBound,
                              i64 NumSamples)
{
    f64 Sum = 0;
    for(i32 Index = 0; Index < NumSamples; ++Index)
    {
        f64 x = RandRange(LowerBound, UpperBound);
        Sum += IntegrandFunction(x);
    }

    f64 Integral = ((UpperBound - LowerBound) / (f64)NumSamples) * Sum;
    printf("The result of integrating the function over the range %f to %f is: "
           "%f.\n",
           LowerBound, UpperBound, Integral);
}

// NOTE: The job of this function is to compute at which X position in the
// graph, the PDF is divided into two equal halves. everything less than this x,
// will have 50% of the area of this curve, everything ahead of this x will also
// be half the area.
// This in short computes the half way point which splits the PDF in half.
void
MC::ComputePDFHalfwayPoint(DoubleInDoubleOut *PDFFunction, f64 LowerBound,
                           f64 UpperBound)
{
    i32 NumSamples = 10'000;
    f64 Sum = 0.0;

    // Iterate through all the samples
    std::vector<sample> Samples;
    for(i32 i = 0; i < NumSamples; ++i)
    {
        f64 x = RandRange(LowerBound, UpperBound);
        f64 p_x = PDFFunction(x);
        Sum += p_x;

        sample ThisSample = {x, p_x};
        Samples.push_back(ThisSample);
    }

    // Sort the Samples
    std::sort(Samples.begin(), Samples.end(), CompareByX);

    // Find the sample at which the area is half
    f64 HalfArea = Sum*0.5;
    f64 HalfwayX = 0.0;
    f64 Accum = 0.;

    for(i32 i = 0; i < NumSamples; ++i)
    {
        sample Sample = Samples[i];
        Accum += Sample.p_x;

        if(Accum >= HalfArea)
        {
            HalfwayX = Sample.x;
            break;
        }
    }

    printf("Average = %f\n", (Sum / NumSamples));
    printf("Area Under Curve = %f\n", 2*pi*(Sum/NumSamples));
    printf("Halfway = %f\n", HalfwayX);
}

// NOTE:
// Here, We will do some pdf stuff and importance sampling to integrate x^2 over
// the range 0-2. Previously we were sampling the x^2 using Rand01() which
// produces uniform values between 0 and 1. What we want is to still uniformly
// sample the x2 function but it should pay more "heed" to important parts of
// the function so that we have less noise and the integral using the MC
// approach converges to the value faster.
// IMPORTANT: The way we do this is we use the inverse sampling method to
// construct a pdf which matches the function in question i.e. x^2 and then
// divide x^2 by that function so that the function we integrate is a constant
// function or close to it because the pdf will not be exactly the function we
// want to integrate.
// First thing is, we want to get the PDF that we want to use to sample the x's
// from. Since the pdf will be similar to the function, we can assume it is
// C*x^2. From the definition of what a pdf is, if we integrate this over the
// range we have here, this integral should evaluate to 1. Integrating C.x^2
// from 0 to 2 equals 1. From this, C = 3/8. So the PDF is 3/8*(x^2). The CDF
// for this pdf is the integral of this pdf. CDF Hence is = 3/8*((x^3) / 3)
// which is 1/8*(x^3). The inverse of this CDF is (8*x)^(1/3). This will be our
// function we will use to sample the x's which we will be using for getting the
// value at that x for the given function. This function let's call it f(d)
// takes in a random number between 0 and 1 uniformly distributed. And gives out
// values between 0 and 2 which is the x value matching the pdf that we want
// here which 3/8*(x^2). Since we are using a non-uniform pdf here to pay "heed"
// to the important parts of the function and using that pdf for sampling, this
// process is called importance sampling.
void
MC::ImportanceSampling()
{
    i32 NumSamples = 1;
    f64 Sum = 0.0;

    auto f = [](f64 x) { return pow(8.0*x, 1.0 / 3.0); };
    auto pdf = [](f64 x) { return (3.0/8.0) * x*x; };

    for(i32 i = 0; i < NumSamples; ++i)
    {
        f64 d = Rand01();
        f64 x = f(d);

        f64 Sample = x*x / (pdf(x));
        Sum += Sample;
    }

    printf("Integral of x^2 over the range 0 to 2 through importance sampling "
           "is : %f.\n",
           (Sum / NumSamples));
}

void
MC::SurfaceIntegralOverSphere()
{
    // NOTE: Here, we want to integrate (cos(theta))^2 over the surface of the
    // sphere. which is Integral(cos^2(Theta)*dA). dA is sin(theta)dTdP for a
    // sphere which over the sphere comes out to be (4/3)*PI. Theta is the polar
    // angle here which is the angle a point on sphere subtends on a z
    // direction which is basically the z part of the unit vector on the sphere.
    // the pdf we are using here is the uniform one which 1/area(of sphere)
    // which is 4*PI.
    auto f = [](const vec3d &d)
    {
        f64 cosine_squared = d.z*d.z;
        return cosine_squared;
    };
    auto pdf = [](const vec3d &d) { return 1.0 / (4.0*pi); };

    i32 NumSamples = 1'000'000;
    f64 Sum = 0.0;

    for(i32 i = 0; i < NumSamples; ++i)
    {
        vec3d d = vec3d::RandomInUnitSphere();
        f64 f_x = f(d);
        f64 Sample = f_x / pdf(d);
        Sum += Sample;
    }

    printf("The surface integral of cosThetaSquared over the unit sphere is: "
           "%f.\n",
           (Sum / NumSamples));
}

#define MONTE_CARLO_H
#endif