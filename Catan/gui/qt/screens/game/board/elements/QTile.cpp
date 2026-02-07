#include <QtMath>
#include <cmath>

#include <types/TypeAliases.h>
#include "QTile.h"

#include <QPainterPath>
#include <common/theme/GameTheme.h>

void QTile::updateGeometry(const QPointF& center, double size) {
    const auto pts = hexPolygonPoints(center, size);
    m_poly.clear();
    m_poly.reserve(6);
    for (const auto& pt : pts) m_poly << pt;

    m_center = center;
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


QVector<QPointF> QTile::hexPolygonPoints(const QPointF& center, double size) {
    // 6 corners, pointy-top => start at -90° so a corner points up.
    QVector<QPointF> pts;
    pts.reserve(6);
    for (int i = 0; i < 6; ++i) {
        const double angleDeg = -90.0 + i * 60.0;
        const double angleRad = qDegreesToRadians(angleDeg);
        pts.push_back({
            center.x() + size * std::cos(angleRad),
            center.y() + size * std::sin(angleRad)
        });
    }
    return pts;
}

static void drawRobber(QPainter& p, const QPointF& c, double size)
{
    p.save();
    p.setRenderHint(QPainter::Antialiasing, true);

    const double r = size * 0.18;
    const QPointF headC(c.x(), c.y() - size*0.06);

    QColor fill(50, 50, 50, 260);
    QPen pen(Qt::black);
    pen.setWidthF(std::max(1.2, size * 0.02));
    p.setPen(pen);
    p.setBrush(fill);

    // head
    p.drawEllipse(headC, r, r);

    // hood/body (rounded triangle)
    QPainterPath hood;
    hood.moveTo(headC.x(), headC.y() - r * 0.9);
    hood.quadTo(QPointF(headC.x() - r*1.3, headC.y() + r*0.2), QPointF(headC.x() - r*1.2, headC.y() + r*1.9));
    hood.quadTo(QPointF(headC.x(),          headC.y() + r*2.4), QPointF(headC.x() + r*1.2, headC.y() + r*1.9));
    hood.quadTo(QPointF(headC.x() + r*1.3, headC.y() + r*0.2), QPointF(headC.x(), headC.y() - r*0.9));
    p.drawPath(hood);

    // tiny “eyes” (optional, comment out if too much)
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(220, 220, 220, 180));
    p.drawEllipse(QPointF(headC.x() - r*0.35, headC.y() - r*0.10), r*0.12, r*0.12);
    p.drawEllipse(QPointF(headC.x() + r*0.35, headC.y() - r*0.10), r*0.12, r*0.12);

    p.restore();
}

void QTile::paint(QPainter& p, double size) {
    if (!m_tile) return;

    p.setBrush(brushFor(m_tile->getResourceType()));
    p.setPen(QPen(Qt::black, 2));
    p.drawPolygon(m_poly);

    if (m_hovered) {
            p.save();
            p.setBrush(QBrush(GameTheme::getGrayColor()));
            p.setPen(Qt::NoPen);
            p.drawPolygon(m_poly);
            p.restore();
        }

    if (m_highlighted) {
        p.save();
        auto col=m_pulse->pulseColor(GameTheme::getGoldenColor());
        col.setAlphaF(0.5);
        p.setBrush(QBrush(col));
        p.setPen(Qt::NoPen);
        p.drawPolygon(m_poly);
        p.restore();
    }

    const bool robberOn = m_tile->isRobberOnTile();
    const bool isDesert = (m_tile->getResourceType() == ResourceType::Desert);
    const int num = m_tile->getNumber();
    const double tokenR = size / 3.0; // token radius

    if (robberOn) {
        if (isDesert) {
            drawRobber(p, m_center, size);
            return;
        }

        p.save();

        p.setBrush(QColor(240, 240, 210));
        p.setPen(QPen(Qt::black, std::max(1.2, size * 0.02)));
        p.drawEllipse(m_center, tokenR, tokenR);

        QPainterPath clip;
        clip.addEllipse(m_center, tokenR * 0.98, tokenR * 0.98);
        p.setClipPath(clip);

        drawRobber(p, m_center, size);

        p.restore();
        return;
    }

    if (!isDesert) {
        p.save();
        p.setBrush(QColor(240, 240, 210));
        p.setPen(QPen(Qt::black, std::max(1.2, size * 0.02)));
        p.drawEllipse(m_center, tokenR, tokenR);

        QFont font("Arial", size / 60.0 * (28 - 2.5 * std::abs(7 - num)));
        p.setFont(font);
        p.setPen((num == 6 || num == 8) ? QColor(255,0,0) : QColor(0,0,0));
        p.drawText(m_tokenRect, Qt::AlignCenter, QString::number(num));

        p.restore();
    }
}

