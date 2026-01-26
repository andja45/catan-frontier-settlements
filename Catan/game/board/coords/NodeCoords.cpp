//
// Created by matija on 1/4/26.
//

#include "NodeCoords.hpp"


NodeCoords::NodeCoords(AxialCoords coords, NodeDirection direction) {
    // multiple tiles share same vertex/edge so we standardize coordinates to canonical form by assigning it to a certain tile
    // for nodes we choose that they belong to tile where they are on the top, top right and bottom right

    // hex with nodes it owns marked with 'x'
    //    x
    // /     \
    // o     x
    // |     |
    // o     o
    // \     /
    //    o
    // (this is best illustration i could do)

    switch (direction) {
    case NodeDirection::Top:
    case NodeDirection::RightTop:
        break;
    case NodeDirection::RightBottom:
        coords = coords.getNeighborCoords(TileDirection::BottomRight);
        direction=NodeDirection::Top;
        break;
    case NodeDirection::Bottom: {
        coords = coords.getNeighborCoords(TileDirection::BottomLeft);
        direction=NodeDirection::RightTop;
        break;
    }
    case NodeDirection::LeftBottom: {
        coords = coords.getNeighborCoords(SideDirection::BottomLeft);
        direction=NodeDirection::Top;
        break;
    }
    case NodeDirection::LeftTop: {
        coords = coords.getNeighborCoords( SideDirection::Left);
        direction=NodeDirection::RightTop;
        break;
    }
    default: ;
    };

    m_q=coords.q();
    m_r=coords.r();
    m_i=static_cast<int>(direction);
}
