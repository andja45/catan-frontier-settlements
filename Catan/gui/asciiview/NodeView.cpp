//
// Created by matija on 12/20/25.
//

#include "NodeView.hpp"


#include <board/Node.h>
#include <game/types/NodeType.h>

void NodeView::draw(Canvas& canvas , const BoardTheme &theme) const {
    char c=0;
    switch (m_node->getNodeType()) {
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
    canvas[m_coord.first][m_coord.second]={c,1};

}
