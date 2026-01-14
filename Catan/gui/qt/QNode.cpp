#include <QtMath>
#include <cmath>

#include <types/TypeAliases.h>
#include <QNode.h>

void QNode::updateGeometry(const QPointF& center, double size) {
    m_center = center;
    const double r = size / 6.0;
    m_path.clear();
    m_path.addEllipse(center, r, r);
}

bool QNode::contains(const QPointF& p) const {
    return m_path.contains(p);
}

void QNode::paint(QPainter& p, double size) {
    if (!m_node) return;

    // hover overlay
    if (m_hovered) {
        p.save();
        p.setBrush(QColor(120, 150, 120, 60));
        p.setPen(Qt::NoPen);
        p.drawPath(m_path);
        p.restore();
    }
}
