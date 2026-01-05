//
// Created by matija on 1/4/26.
//

#include "EdgeCoords.hpp"
#include "NodeCoords.hpp"

// see explanation for node coords
class EdgeCoords EdgeCoords::standardize() const {
    //0 1 2 same the same
    //3,4,5 go to next hex and change index to 0,1,2

    EdgeDirection new_dir=EdgeDirection::End;
    AxialCoords new_coords=this->getAxialCoords();
    switch (direction()) {
        case EdgeDirection::TopRight:
        case EdgeDirection::Right:
        case EdgeDirection::BottomRight:
            return *this;
        case EdgeDirection::BottomLeft:
        case EdgeDirection::Left:
        case EdgeDirection::TopLeft: {
            new_coords = new_coords.getNeighborCoords(oppositeDir());
            new_dir=direction();
            break;
        }
    }
    return EdgeCoords(new_coords,new_dir);
}

EdgeDirection EdgeCoords::oppositeDir() const {
    return this->next().next().next().direction();
}

EdgeCoords EdgeCoords::next() const {
    int edgeCount=static_cast<int>(EdgeDirection::End);
    EdgeDirection nextDir = static_cast<EdgeDirection>((static_cast<int>(direction())+1)%edgeCount);
    return EdgeCoords(this->getAxialCoords(),nextDir);
}

NodeCoords EdgeCoords::getNodeCoordAfter() {
    NodeDirection dir1=static_cast<NodeDirection>(this->m_i);
    return this->getAxialCoords().getNodeCoordsAt(dir1);
}

NodeCoords EdgeCoords::getNodeCoordBefore() {

    NodeDirection dir2=static_cast<NodeDirection>(this->next().m_i);
    return this->getAxialCoords().getNodeCoordsAt(dir2);
}
