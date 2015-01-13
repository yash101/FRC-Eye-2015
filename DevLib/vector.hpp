#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <cmath>
#include <string>
#include <cstring>
#include <sstream>
#include "point.hpp"
namespace dev
{
    template<typename T> class Vector
    {
    private:
        T _x;
        T _y;
        dev::Vector<T> getVector() { return dev::Vector<T>(_y, _y); }
    public:
        ////Object access
        //Get/set OOP programming stuffs
        T getX() { return _x; }
        T getY() { return _y; }
        template<typename Type> void setX(Type type) { _x = type; }
        template<typename Type> void setY(Type type) { _y = type; }

        ////Constructors
        //Basic constructors
        Vector() {}
        Vector(T x, T y) { _x = x; _y = y; }
        Vector(T x1, T y1, T x2, T y2) { _x = x2 - x2; _y = y2 - y1; }
        template<typename Type1, typename Type2> Vector(dev::Point<Type1> pt1, dev::Point<Type2> pt2)
            { _x = pt2.getX() - pt1.getX(); _y = pt2.getY() - pt1.getY(); }
        //Copy constructors
        template<typename Type> Vector(const dev::Vector<Type>& vec) { _x = vec.getX(); _y = vec.getY(); }
        template<typename Type> Vector& operator=(const Vector<Type>& vec) { _x = vec.getX(); _y = vec.getY(); }
        //Move constructor
        template<typename Type> Vector(dev::Vector<Type>&& vec) { _x = vec.getX(); _y = vec.getY(); vec.setX(0), vec.setY(0); }

        ////Logic operators
        template<typename Type> bool operator==(dev::Vector<Type> vec) { return _x == vec.getX() && _y == vec.getY(); }
        template<typename Type> bool operator!=(dev::Vector<Type> vec) { return !(_x == vec.getX() && _y == vec.getY()); }


        ////Mathematics of vectors

        //Dot product of two vectors
        template<typename Type> T operator*(dev::Vector<Type> vec) { return (_x * vec.getX()) + (_y * vec.getY()); }    //Returns class type
        //Scaling of two vectors
        template<typename Type> dev::Vector<T> operator*(Type type) { return dev::Vector<T>(_x * type, _y * type); }    //Returns scaled Vector

        //Add two vectors
        template<typename Type> dev::Vector<T> operator+(dev::Vector<Type> vec) { return dev::Vector<T>(_x + vec.getX(), _y + vec.getY()); }

        //Substract two vectors
        template<typename Type> dev::Vector<T> operator-(dev::Vector<Type> vec) { return dev::Vector<Type>(_x - vec.getX(), _y - vec.getY()); }

        //Division of two vectors
        template<typename Z> dev::Vector<T> operator/(Z divisor) { return dev::Vector<T>(_x / (T) divisor, _y / (T) divisor); }

        //Magnitude
        T magnitude() { return sqrt((_x * _x) + (_y * _y)); }

        //Angle calculation
        template<typename Type> T angle(dev::Vector<Type> vec) { return (dev::Vector<T>(_x, _y) * vec) / (dev::Vector<T>(_x, _y).magnitude() * vec.magnitude()); }

        //Unit vectors
        dev::Vector<T> unitVector() { return dev::Vector<T>(_x, _y) / magnitude(); }

        ////Pretty stuff
        //Output a string with this converted to component form (<1, 1>)
        std::string componentForm()
        {
            return toString(true);
        }

        //Output a string with this converted to standard form (i + j)
        std::string standardForm()
        {
            return toString(false);
        }

        //Component form and standard form functions together
        std::string toString(bool ComponentForm)
        {
            if(ComponentForm)
            {
                std::stringstream str;
                str << "<" << _x << ", " << _y << ">";
                return str.str();
            }
            else
            {
                std::stringstream str;
                str << _x << "i";
                if(_y > 0)
                {
                    str << " + " << _y << "j";
                }
                else
                {
                    str << " - " << 0 - _y << "j";
                }
                return str.str();
            }
        }
    };

    //Some typedef's for typical vector types. Just to make coding a bit simpler to understand
    typedef dev::Vector<float> Vec2f;
    typedef dev::Vector<int> Vec2i;
    typedef dev::Vector<double> Vec2d;
    typedef dev::Vector<long double> Vec2ld;
    typedef dev::Vector<long> Vec2l;
    typedef dev::Vector<long long> Vec2ll;
}
#endif // VECTOR_HPP
