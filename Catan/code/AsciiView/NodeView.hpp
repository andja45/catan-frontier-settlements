//
// Created by matija on 12/20/25.
//

#ifndef CATAN_NODEVIEW_HPP
#define CATAN_NODEVIEW_HPP
#include <utility>

#include "BoardView.hpp"

class NodeView {
private:
    Node* m_node=nullptr;
    std::pair<int,int> m_coord;
public:
    NodeView(Node* n, const std::pair<int, int> &m_coord)
    : m_coord(m_coord), m_node(n) {
    }
    void draw(Canvas &canvas, const BoardTheme &theme) const;
};

#endif //CATAN_NODEVIEW_HPP