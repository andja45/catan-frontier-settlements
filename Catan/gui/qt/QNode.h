#ifndef QNODE_H
#define QNODE_H

#include <QPolygonF>
#include <QPointF>
#include <QPainter>
#include <QPainterPath>

#include <board/Node.h>

class QNode {
public:
    explicit QNode(Node* model = nullptr) : m_node(model) {}

    Node* node() const { return m_node; }

    void updateGeometry(const QPointF& center, double size);
    bool contains(const QPointF& p) const;

    void setHovered(bool on) { m_hovered = on; }
    bool hovered() const { return m_hovered; }

    // draw hover + settlement/city
    void paint(QPainter& p, double size);

    bool handleClick(PlayerId player);

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
};

#endif // QNODE_H
