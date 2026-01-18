//
// Created by matija on 12/20/25.
//

#ifndef CATAN_NODEVIEW_HPP
#define CATAN_NODEVIEW_HPP
#include <utility>
#include <drawing/AsciiDrawable.hpp>

#include "../drawing/AsciiTypes.hpp"
struct BoardTheme;
class Node;

class NodeView : public AsciiDrawable {
private:
    Node* m_node=nullptr;
    ScreenCoords m_coord;
public:
    NodeView(Node* n, const ScreenCoords &m_coord)
    : AsciiDrawable(m_coord,{1,1}), m_node(n) {
    }
    void render(Canvas &canvas) const override;

};

#endif //CATAN_NODEVIEW_HPP