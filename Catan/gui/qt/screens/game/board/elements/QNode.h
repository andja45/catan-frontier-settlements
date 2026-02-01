#ifndef QNODE_H
#define QNODE_H

#include <QPointF>
#include <QPainterPath>

#include <board/Node.h>

class QNode {
public:
    bool highlighted = true;

    explicit QNode(Node* node = nullptr) : m_node(node) {}

    Node* node() const { return m_node; }

    void updateGeometry(const QPointF& center, double size);
    bool contains(const QPointF& p) const;

    void setHovered(bool on) { m_hovered = on; }
    bool hovered() const { return m_hovered; }

    // draw hover + settlement/city
    void paint(QPainter& p, double size);


    void setHighlighted(bool on) { m_highlighted = on; }
    bool isHighlighted() const { return m_highlighted; }

private:
    bool hoverAllowed() const; // empty or settlement
    void drawSettlement(QPainter& p, double size);
    void drawCity(QPainter& p, double size);
    QColor playerColor() const; // placeholder

private:
    Node*       m_node = nullptr;  // non-owning
    QPointF     m_center;
    QPainterPath m_hit;            // for hit-test
    bool        m_hovered = false;
    bool        m_highlighted = false;
};

#endif // QNODE_H
