#include "QNode.h"
#include <cmath>
#include <QPolygonF>
#include <QPainter>
#include <common/theme/GameTheme.h>

bool QNode::hoverAllowed() const {
    if (!m_node) return false;
    return m_node->isEmpty() || m_node->isSettlement();
}

QColor QNode::playerColor() const {
    auto owner = m_node->getOwner();
    if (owner != types::InvalidPlayerId)
        return GameTheme::playerColors.at(owner);
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

void QNode::drawSettlement(QPainter& p, double size) {
    const double baseW     = size * 0.40;
    const double baseH     = size * 0.30;

    const double overhang  = baseW * 0.22;
    const double roofH     = baseH * 0.70;
    const double roofOffset = baseW * 0.06;

    QRectF base(
        m_center.x() - baseW / 2.0,
        m_center.y() - baseH / 2.0 + baseH * 0.18,
        baseW,
        baseH
    );

    const QPointF roofLeft (base.left()  - overhang, base.top());
    const QPointF roofRight(base.right() + overhang, base.top());
    const QPointF roofPeak (m_center.x(), base.top() - roofH);

    QPainterPath roofPath;
    roofPath.moveTo(roofLeft);
    roofPath.lineTo(roofPeak);
    roofPath.lineTo(roofRight);
    roofPath.lineTo(QPointF(base.right() - roofOffset, base.top()));
    roofPath.lineTo(QPointF(base.left()  + roofOffset, base.top()));
    roofPath.closeSubpath();

    const double doorW = baseW * 0.26;
    const double doorH = baseH * 0.48;
    QRectF door(
        m_center.x() - doorW / 2.0,
        base.bottom() - doorH,
        doorW,
        doorH
    );

    p.save();
    p.setRenderHint(QPainter::Antialiasing, true);

    const QColor baseCol = playerColor();
    const QColor roofCol = baseCol.darker(120);

    QPen outline(Qt::black);
    outline.setWidthF(std::max(1.2, size * 0.03));
    p.setPen(outline);

    p.setBrush(baseCol);
    p.drawRect(base);

    p.setBrush(roofCol);
    p.drawPath(roofPath);

    p.setBrush(baseCol);
    p.drawRect(door);

    p.restore();
}

void QNode::drawCity(QPainter& p, double size){
    const double baseW = size * 0.42;
    const double baseH = size * 0.34;

    QRectF base(
        m_center.x() - baseW / 2.0,
        m_center.y() - baseH / 2.0 + baseH * 0.12,
        baseW,
        baseH
    );

    const double roofH     = baseH * 0.42;
    const double roofOver  = baseW * 0.12;
    const double roofInset = baseW * 0.18;

    QPolygonF roof;
    roof << QPointF(base.left()  - roofOver,                 base.top())
         << QPointF(base.right() + roofOver,                 base.top())
         << QPointF(base.right() + roofOver - roofInset,     base.top() - roofH)
         << QPointF(base.left()  - roofOver + roofInset,     base.top() - roofH);

    const double padX = baseW * 0.14;
    const double padY = baseH * 0.18;
    const double gapX = baseW * 0.10;
    const double gapY = baseH * 0.10;

    const double winW = (baseW - 2*padX - gapX) / 2.0;
    const double winH = (baseH - 2*padY - gapY) / 2.0;

    QRectF w1(base.left() + padX,              base.top() + padY,              winW, winH);
    QRectF w2(base.left() + padX + winW+gapX,  base.top() + padY,              winW, winH);
    QRectF w3(base.left() + padX,              base.top() + padY + winH+gapY,  winW, winH);
    QRectF w4(base.left() + padX + winW+gapX,  base.top() + padY + winH+gapY,  winW, winH);

    p.save();
    p.setRenderHint(QPainter::Antialiasing, true);

    const QColor baseCol = playerColor();
    const QColor roofCol = baseCol.darker(120);
    QColor winCol = baseCol.lighter(130);
    winCol.setHsv(
        winCol.hue(),
        winCol.saturation() * 0.35,
        winCol.value()
    );
    const QColor winFrame = QColor(0, 0, 0, 160);

    QPen outline(Qt::black);
    outline.setWidthF(std::max(1.2, size * 0.03));
    p.setPen(outline);

    p.setBrush(baseCol);
    p.drawRect(base);

    p.setBrush(roofCol);
    p.drawPolygon(roof);

    p.setBrush(winCol);
    p.setPen(QPen(winFrame, std::max(1.0, size * 0.015)));
    p.drawRect(w1); p.drawRect(w2); p.drawRect(w3); p.drawRect(w4);

    p.restore();
}

void QNode::paint(QPainter& p, double size) {
    if (!m_node) return;

    // Draw settlement/city if present
    if (m_node->isCity()) {
        drawCity(p, size);
    } else if (m_node->isSettlement()) {
        drawSettlement(p, size);
    }

    // Hover ring: only over empty or settlement nodes
    if (m_hovered && hoverAllowed()) {
        p.save();
        const double r = size / 4.2;

        // Ring (no fill)
        QPen ring(GameTheme::getGrayColorStrong());
        ring.setWidthF(std::max(2.0, size / 30.0));
        p.setPen(ring);
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(m_center, r, r);

        // Subtle fill
        p.setPen(Qt::NoPen);
        p.setBrush(GameTheme::getGrayColor());
        p.drawEllipse(m_center, r, r);

        p.restore();
    }

    if (m_highlighted) {
        p.save();
        const double r = size / 4.2;

        // Ring (no fill)

        QPen ring(GameTheme::getGoldenColorStrong());
        ring.setWidthF(std::max(2.0, size / 30.0));
        p.setPen(ring);
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(m_center, r, r);

        // Subtle fill
        p.setPen(Qt::NoPen);
        auto col=m_pulse->pulseColor(GameTheme::getGoldenColor());
        p.setBrush(col);
        p.drawEllipse(m_center, r, r);

        p.restore();
    }
}
