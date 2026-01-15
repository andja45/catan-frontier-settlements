#include "QNode.h"
#include <cmath>

// include your model node header
#include <board/Node.h>  // adjust include path

bool QNode::hoverAllowed() const {
    if (!m_node) return false;
    return m_node->isEmpty() || m_node->isSettlement();
}

QColor QNode::playerColor() const {
    return QColor(40, 40, 40);
}

void QNode::updateGeometry(const QPointF& center, double size) {
    m_center = center;
    const double r = size / 4.5;
    m_hit = QPainterPath();
    m_hit.addEllipse(center, r, r);
}

bool QNode::contains(const QPointF& p) const {
    return m_hit.contains(p);
}

bool QNode::handleClick(PlayerId player) {
    if (!m_node) return false;

    if (m_node->isEmpty()) {
        m_node->buildSettlement(player);
        return true;
    }
    if (m_node->isSettlement()) {
        m_node->buildCity();
        return true;
    }
    return false; // city -> nothing
}

void QNode::drawSettlement(QPainter& p, double size) {
    // A small triangle ("house") centered on m_center
    const double h = size / 3.6;  // height
    const double w = size / 3.8;  // width

    QPolygonF tri;
    tri << QPointF(m_center.x(),         m_center.y() - h * 0.65)
        << QPointF(m_center.x() - w/2.0, m_center.y() + h * 0.35)
        << QPointF(m_center.x() + w/2.0, m_center.y() + h * 0.35);

    p.save();
    p.setBrush(playerColor());
    p.setPen(QPen(Qt::black, 1.5));
    p.drawPolygon(tri);
    p.restore();
}

void QNode::drawCity(QPainter& p, double size) {
    // A small blocky "castle" (rectangle + roof bump)
    const double w = size / 3.4;
    const double h = size / 3.1;

    QRectF base(m_center.x() - w/2.0, m_center.y() - h/2.0, w, h);

    // Add a little "roof" rectangle on top to distinguish from settlement
    QRectF top(m_center.x() - w*0.30, m_center.y() - h*0.70, w*0.60, h*0.35);

    p.save();
    p.setBrush(playerColor().lighter(115));
    p.setPen(QPen(Qt::black, 1.5));
    p.drawRect(base);
    p.drawRect(top);
    p.restore();
}

void QNode::paint(QPainter& p, double size) {
    if (!m_node) return;

    // Hover ring: only over empty or settlement nodes
    if (m_hovered && hoverAllowed()) {
        p.save();
        const double r = size / 3.2;

        // Ring (no fill)
        QPen ring(QColor(255, 255, 255, 200));
        ring.setWidthF(std::max(2.0, size / 30.0));
        p.setPen(ring);
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(m_center, r, r);

        // Subtle fill
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(255, 255, 255, 40));
        p.drawEllipse(m_center, r, r);

        p.restore();
    }

    // Draw settlement/city if present
    if (m_node->isCity()) {
        drawCity(p, size);
    } else if (m_node->isSettlement()) {
        drawSettlement(p, size);
    }
}
