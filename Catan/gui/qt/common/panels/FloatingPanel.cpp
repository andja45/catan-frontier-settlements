#include "FloatingPanel.h"

#include <QPainter>

void FloatingPanel::paintEvent(QPaintEvent* e){
    QWidget::paintEvent(e);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QRectF r = rect().adjusted(1, 1, -1, -1);
    const qreal radius = 12.0;

    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 22));
    p.drawRoundedRect(r, radius, radius);

    if (!m_activeHighlight)
        return;

    const QRectF hr = rect().adjusted(2, 2, -2, -2);

    for (int i = 0; i < 3; ++i) {
        QPen glow(QColor(80, 170, 255, 90 - i * 25));
        glow.setWidth(6 - i * 2);
        glow.setJoinStyle(Qt::RoundJoin);
        p.setPen(glow);
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(hr, radius, radius);
    }

    QPen edge(QColor(80, 170, 255, 230));
    edge.setWidth(2);
    edge.setJoinStyle(Qt::RoundJoin);
    p.setPen(edge);
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(hr, radius, radius);
}
