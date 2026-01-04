//
// Created by matija on 1/4/26.
//

#include "NodeCoords.hpp"


// helper functions to standardize coordinates of nodes and edges
// multiple tiles share same vertex/edge so we standardize coordinates to canonical form by assigning it to a certain tile
// about axial coordinate system: https://www.redblobgames.com/grids/hexagons/
NodeCoords NodeCoords::standardize() const {
    //0 1 2 stay the same
    //3,4,5 go to next hex and change index to 0,1,2

    NodeDirection new_dir=NodeDirection::End;
    AxialCoords new_coords=this->getAxialCoords();
    switch (direction()) {
        case NodeDirection::Top:
        case NodeDirection::RightTop:
        case NodeDirection::RightBottom:
            return *this;
        case NodeDirection::Bottom: {
            new_coords = this->getAxialCoords().getNeighborCoords(SideDirection::BottomLeft);
            new_dir=NodeDirection::RightTop;
            break;
        }
        case NodeDirection::LeftBottom: {
            new_coords = this->getAxialCoords().getNeighborCoords(SideDirection::BottomLeft);
            new_dir=NodeDirection::Top;
            break;
        }
        case NodeDirection::LeftTop: {
            new_coords = this->getAxialCoords().getNeighborCoords( SideDirection::Left);
            new_dir=NodeDirection::RightTop;
            break;
        }
    };
    return NodeCoords(new_coords,new_dir);
}


NodeCoords NodeCoords::next() const {
    int nodeCount=static_cast<int>(NodeDirection::End);
    NodeDirection new_i=static_cast<NodeDirection>((static_cast<int>(direction())+1)%nodeCount);
    return NodeCoords(getAxialCoords(),new_i);
}
