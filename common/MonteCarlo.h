#if !defined(MONTE_CARLO_H)

#include "defines.h"

#define FUNC(name) f64 name(f64 x)
typedef FUNC(DoubleInDoubleOut);

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
};

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

#define MONTE_CARLO_H
#endif