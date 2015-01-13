#ifndef POINT_HPP
#define POINT_HPP
#include <cmath>
namespace dev
{
    template<typename T> class Point
    {
    private:
        T _x;
        T _y;
    public:
        //Constructors
        //-/Default constructor. Takes X and Y
        Point(T x, T y) { _x = x; _y = y; }
//        //-/Move constructor
//        template<typename type> Point(dev::Point<type>&& pt) { _x = pt.getX(); _y = pt.getY(); }
        //-/Assignment operator construction
        template<typename type> void operator=(dev::Point<type>& pt) { _x = pt.getX(); _y = pt.getY(); }

        //Getter's and Setter's
        //-/Rectangular
        T getX() { return _x; }
        T getY() { return _y; }
        template<typename Z> void setX(Z x) { _x = (T) x; }
        template<typename Z> void setY(Z y) { _y = (T) y; }
        //-/Polar
        T getR() { return _x; }
        T getTheta() { return _y; }
        template<typename Z> void setR(Z r) { _x = (T) r; }
        template<typename Z> void setTheta(Z theta) { _y = (T) theta; }

        //Comparison operators
        template<typename type> bool operator==(dev::Point<type>& pt) { return _x == pt.getX() && _y == pt.getY(); }
        template<typename type> bool operator!=(dev::Point<type>& pt) { return _x != pt.getX() || _y != pt.getY(); }

        //Calculate the distance between a different point
        template<typename type> T distance(dev::Point<type>& pt)
            { return std::sqrt(((pt.getX() - _x) * (pt.getX - _x)) + ((pt.getY() - _y) * (pt.getY() - _y))); }

        //Polar and cartesian conversions
        dev::Point<T> toPolar() { return dev::Point<T>(std::sqrt((_x * _x) + (_y * _y)), std::atan(_y / _x)); }
        dev::Point<T> toRectangular() { return dev::Point<T>(_x * std::cos(_y), _x * std::sin(_y)); }
    };

    //Convert rectangular coordinates (point) to polar
    template<typename T> dev::Point<T> toPolar(dev::Point<T> p)
    {
        return dev::Point<T>(std::sqrt((p.getX() * p.getX()) + (p.getY() * p.getY())), std::atan(p.getY() / p.getX()));
    }

    //Convert polar coordinates (point) to retangular (cartesian)
    template<typename T> dev::Point<T> toRectangular(dev::Point<T> p)
    {
        return dev::Point<T>(p.getX() * std::cos(p.getY()), p.getY() * std::sin(p.getY()));
    }
}
#endif // POINT_HPP
