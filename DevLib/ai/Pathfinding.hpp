#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP
#include <iostream>
#include <vector>
#include "../point.hpp"
#include "../Exceptions.hpp"

namespace dev
{
    template<typename T>class Node
    {
        T G;
        T H;
        dev::Node<T> *child;
        T F() { return G + H; }
    };
    typedef dev::Node<float> Nodef;
    typedef dev::Node<int> Nodei;
}
#endif // PATHFINDING_HPP
