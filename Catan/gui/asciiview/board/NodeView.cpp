//
// Created by matija on 12/20/25.
//

#include "NodeView.hpp"


#include <board/Node.h>
#include <drawing/AsciiCanvas.hpp>
#include <drawing/AsciiTheme.hpp>
#include <types/NodeType.h>

void NodeView::render(Canvas& canvas ) const {
    const auto& theme=BoardTheme::getInstance();
    char c=0;
    switch (m_node->getNodeBuildingType()) {
    case NodeType::Settlement:
        c=theme.settlementChar;
        break;
    case NodeType::City:
        c=theme.cityChar;
        break;
    case NodeType::None:
        c=theme.settlementChar;
        break;
    }
    canvas.setCell(m_coord,{c,1});

}
