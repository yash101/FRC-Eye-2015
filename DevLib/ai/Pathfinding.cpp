#include "Pathfinding.hpp"
#include <fstream>
#include <cstring>
#include "../misc.hpp"

//std::vector<dev::Point<int> > dev::AStar::Grid::getNeighbors(dev::Point<int> pt)
//{
//    //Add the possible points
//    std::vector<dev::Point<int> > consider;
//    consider.push_back(dev::Point<int>(pt.getX() + 1, pt.getY()));       // 1        O O O   //O = add X = location
//    consider.push_back(dev::Point<int>(pt.getX() - 1, pt.getY()));       // 2        O X O
//    consider.push_back(dev::Point<int>(pt.getX(), pt.getY() + 1));       // 3        O O O
//    consider.push_back(dev::Point<int>(pt.getX(), pt.getY() - 1));       // 4        -----
//    consider.push_back(dev::Point<int>(pt.getX() + 1, pt.getY() + 1));   // 5        7 3 5   //Order of the points
//    consider.push_back(dev::Point<int>(pt.getX() + 1, pt.getY() - 1));   // 6        2 X 1
//    consider.push_back(dev::Point<int>(pt.getX() - 1, pt.getY() + 1));   // 7        8 4 6
//    consider.push_back(dev::Point<int>(pt.getX() - 1, pt.getY() - 1));   // 8

//    //Check to ensure validity of the points
//    std::vector<dev::Point<int> > validate;
//    for(unsigned int i = 0; i < consider.size(); i++)
//    {
//        if(isOnGrid(consider[i])) { validate.push_back(consider[i]); }
//    }

//    return validate;
//}

//std::vector<dev::Point<int> > dev::AStar::generatePath(dev::AStar::Grid &grid, dev::Point<int> start, dev::Point<int> end)
//{
//    //------Preparation/Initialization
//    //Ensure the our start and end points are on the grid. If they aren't, throw an exception because someone screwed up ;)
//    if(!grid.isOnGrid(start) || !grid.isOnGrid(end))
//        { throw AIException("One or more of the points to generate a path lie outside the grid!"); }

//    //------Processing
//    //Open and closed cells lists
//    std::vector<dev::AStar::Cell> open;
//    std::vector<dev::AStar::Cell> closed;

//    //To reduce redundancy and make the code cleaner. Just use find and replace and replace Cell with dev::AStar::Cell
//    //and Point with dev::Point and pti with Point<int> to get the typical look!
//    using dev::AStar::Cell;
//    using dev::Point;
//    typedef Point<int> pti;
//    typedef unsigned int uint;

//    //----Finally, the search!
//    //Put our starting position in the open list. We **WILL** transverse this!
//    open.push_back(Cell(start.getX(), start.getY()));
//    std::vector<pti> x = grid.getNeighbors(start);

//    for(uint i = 0; i < x.size(); i++)
//    {
//        open.push_back(Cell(x[i]));
//    }
//}
