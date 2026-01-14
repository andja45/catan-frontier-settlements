#ifndef QNODE_H
#define QNODE_H

#include <QPolygonF>
#include <QPointF>
#include <QPainter>
#include <QPainterPath>

#include <board/Node.h>

class QNode {
public:
    explicit QNode(Node* node = nullptr) : m_node(node) {}

    Node* node() const { return m_node; }

    void updateGeometry(const QPointF& center, double size);
    bool contains(const QPointF& p) const;

    void setHovered(bool on) { m_hovered = on; }
    bool hovered() const { return m_hovered; }

    void paint(QPainter& p, double size);

private:
    Node*     m_node = nullptr;
    QPointF   m_center;
    bool      m_hovered = false;
    QPainterPath m_path;
};

#endif // QNODE_H
