#include <QtMath>
#include <cmath>

#include <types/TypeAliases.h>
#include <QTile.h>

void QTile::updateGeometry(const QPointF& center, const QPolygonF& poly, double size) {
    m_center = center;
    m_poly = poly;
    const double r = size / 3.0;
    m_tokenRect = QRectF(center.x() - r, center.y() - r, 2*r, 2*r);
}

bool QTile::contains(const QPointF& p) const {
    return m_poly.containsPoint(p, Qt::OddEvenFill);
}

static QBrush brushFor(ResourceType t) {
    switch (t) {
    case ResourceType::Wood:   return QBrush(QColor(34, 139, 34));
    case ResourceType::Brick:  return QBrush(QColor(178, 74, 34));
    case ResourceType::Ore:    return QBrush(QColor(160, 160, 160));
    case ResourceType::Wool:   return QBrush(QColor(144, 238, 144));
    case ResourceType::Wheat:  return QBrush(QColor(230, 205, 22));
    case ResourceType::Desert: return QBrush(QColor(200, 165, 112));
    case ResourceType::Sea:    return QBrush(QColor(80, 140, 200));
    default:                  return QBrush(Qt::NoBrush);
    }
}

void QTile::paint(QPainter& p, double size, bool placingRobber) {
    if (!m_tile) return;

    // fill
    p.setBrush(brushFor(m_tile->getResourceType()));
    p.drawPolygon(m_poly);

    // hover overlay (robber mode)
    if (placingRobber && m_hovered) {
        p.save();
        p.setBrush(QBrush(QColor(255, 255, 255, 60)));
        p.setPen(Qt::NoPen);
        p.drawPolygon(m_poly);
        p.restore();
    }

    // number token
    const int num = m_tile->getNumber();
    if (num == 7) return;

    p.save();
    p.setBrush(QColor(240, 240, 210));
    p.drawEllipse(m_center, size / 3.0, size / 3.0);

    QFont font("Arial", size / 60.0 * (28 - 2.5 * std::abs(7 - num)));
    p.setFont(font);
    p.setPen((num == 6 || num == 8) ? QColor(255,0,0) : QColor(0,0,0));

    p.drawText(m_tokenRect, Qt::AlignCenter, QString::number(num));
    p.restore();
}
