#ifndef MATH_HPP
#define MATH_HPP
#include <cmath>
#include <iostream>
#ifndef PI
#define PI 3.14159256
#endif
#define DegToRadConstant PI / 180.f
#define RadToDevConstant 180.f / PI
#include "compat.hpp"
#ifdef CXX11_SUPPORTED
#include <functional>
#endif
//Shoud we dynamically calculate PI at program launch? Pretty useless!
#ifdef CALCULATE_PI_DYNAMIC
#ifndef CALCULATE_PI_DYNAMIC_PRECISION
#define CALCULATE_PI_DYNAMIC_PRECISION  100000
#endif
#endif
namespace dev
{
    //Trigonometric conversion from radians to degrees
    template<typename Y> Y radiansToDegrees(Y x) { return x * ((Y) 180 / (Y) PI); }

    //Trigonometric conversion from degrees to radians
    template<typename Y> Y degreesToRadians(Y x) { return x * ((Y) PI / (Y) 180); }

    //Distance calculations for image processing
    template<typename A, typename B, typename C, typename D, typename E> E Distance(A FOV, B ObjectLength, C MeasuredLength, D ResolutionLength)
    { return (E) ((ResolutionLength * ObjectLength) / MeasuredLength) / std::tan((FOV / 2.f) * (PI / 180.f)); }

    //Pythagorean theorem to calculate a hypotenuse
    template<typename T, typename X> T PythagoreanHypotenuse(T sideA, X sideB)
    { return std::sqrt((sideA * sideA) + (sideB * sideB)); }

    template<typename T, typename X> T PythagoreanLeg(T sideA, X sideB)
    { return std::sqrt(std::abs((sideA * sideA) - (sideB * sideB))); }

    template<typename A> A abs(A number)
    {
        if(number < 0)
        {
            return 0.f - number;
        }
        else
        {
            return number;
        }
    }

    template<typename T> inline T piCalc(size_t iterations)
    {
        long long lastnum = 1;
        T out = 0;
        bool x = true;
        for(size_t i = 0; i < iterations; i++)
        {
            if(x)
            {
                out += ((T) 4 / (T) lastnum);
            }
            else
            {
                out -= ((T) 4 / (T) lastnum);
            }
            lastnum += 2;
            x = !x;
        }
        return out;
    }

    template<typename FPA> FPA modulus(FPA number, FPA threshmax)
    {
        long long x = (FPA) number / (FPA) threshmax;
        FPA ttlnum = x * threshmax;
        return number - ttlnum;
    }

    inline long long factorial(char number)
    {
        long long out = 1;
        for(long long i = 1; i <= number; i++)
        {
            out *= i;
        }
        return out;
    }

//This will only work with C++11+ (std::function())
#ifdef CXX11_SUPPORTED
    template<typename FPT> FPT summation(int start, int end, std::function<FPT(int iteration)> funct)
    {
        FPT outcome = (FPT) 0;
        if(end < start)
        {
            for(int i = start; i >= end; i--)
            {
                outcome += funct(i);
            }
        }
        else
        {
            for(int i = 0; i <= end; i++)
            {
                outcome += funct(i);
            }
        }
        return outcome;
    }
#endif

#ifdef CALCULATE_PI_DYNAMIC
    const static long double pi = dev::piCalc<long double>(100000);
#else
    //Sorry, I like to get kinda retarded-accurate lol! This number was calculated in 32.1 minutes by the
    //dev::piCalc() function!
    const static long double pi = 3.14159265158979399070078575295639211617526598274707794189453125;
#endif

    template<typename FPA> FPA sin(FPA traj)
    {
        //Calculate PI!!! :D
        return dev::summation<FPA>(0, 12, [&](int i)
        {
            return (std::pow(-1.0, i) / dev::factorial((char) 2.0 * i) + 1.0) * std::pow(dev::modulus(traj, (FPA) dev::pi), (2.0 * i) + 1);
        });
    }

    template<typename FPA> FPA cos(FPA traj)
    {
        return dev::sin<FPA>(traj + dev::pi / 2);
    }

    template<typename FPA> FPA tan(FPA traj)
    {
        return dev::sin<FPA>(traj) / dev::cos<FPA>(traj);
    }

    template<typename FPA> FPA csc(FPA traj) { return (FPA) 1.0 / dev::sin<FPA>(traj); }
    template<typename FPA> FPA sec(FPA traj) { return (FPA) 1.0 / dev::cos<FPA>(traj); }
    template<typename FPA> FPA cot(FPA traj) { return (FPA) 1.0 / dev::tan<FPA>(traj); }
}
#endif // MATH_HPP
