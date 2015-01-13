#ifndef MATH_HPP
#define MATH_HPP
#include <cmath>
#include <iostream>
#ifndef PI
#define PI 3.14159256
#endif
#define DegToRadConstant PI / 180.f
#define RadToDevConstant 180.f / PI
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
}
#endif // MATH_HPP
