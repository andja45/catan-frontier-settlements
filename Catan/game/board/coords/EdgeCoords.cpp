//
// Created by matija on 1/4/26.
//

#include "EdgeCoords.hpp"
#include "NodeCoords.hpp"


EdgeCoords::EdgeCoords(AxialCoords coords, EdgeDirection direction) {
    // edge coords need to be standardised
    // see explanation for node coords

    switch (direction) {
    // each hex "owns" right side of edges
    case EdgeDirection::TopRight:
    case EdgeDirection::Right:
    case EdgeDirection::BottomRight:
        break;
    // for edges on left we go in the opposite direction and get neighbor coords
    case EdgeDirection::BottomLeft:
    case EdgeDirection::Left:
    case EdgeDirection::TopLeft: {
        int count = static_cast<int>(EdgeDirection::End);
        EdgeDirection oppositeDir=(directions::oppositeSide(direction));
        coords = coords.getNeighborCoords(direction);
        direction=oppositeDir;
        break;
    }
    default: ;
    }

    m_q=coords.q();
    m_r=coords.r();
    m_i=static_cast<int>(direction);
}
