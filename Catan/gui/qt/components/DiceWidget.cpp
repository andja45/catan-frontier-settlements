#include "DiceWidget.h"
DiceWidget::DiceWidget(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
}

void DiceWidget::setDice(int d1, int d2)
{
    m_die1 = qBound(1, d1, 6);
    m_die2 = qBound(1, d2, 6);
    update();
}

void DiceWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const int spacing = 10;
    const int size = qMin(height(), (width() - spacing) / 2);

    QRectF die1Rect(0, (height() - size) / 2, size, size);
    QRectF die2Rect(size + spacing, (height() - size) / 2, size, size);

    drawDie(p, die1Rect, m_die1);
    drawDie(p, die2Rect, m_die2);
}

void DiceWidget::drawDie(QPainter& p, const QRectF& r, int value)
{
    // Die background
    p.setBrush(QColor(250, 250, 250));
    p.setPen(QColor(0, 0, 0, 120));
    p.drawRoundedRect(r, 8, 8);

    p.setBrush(Qt::black);
    p.setPen(Qt::NoPen);

    auto pip = [&](qreal x, qreal y) {
        const qreal s = r.width() * 0.12;
        p.drawEllipse(QPointF(x, y), s, s);
    };

    const qreal cx = r.center().x();
    const qreal cy = r.center().y();
    const qreal o  = r.width() * 0.25;

    switch (value) {
    case 1:
        pip(cx, cy);
        break;
    case 2:
        pip(cx - o, cy - o);
        pip(cx + o, cy + o);
        break;
    case 3:
        pip(cx, cy);
        pip(cx - o, cy - o);
        pip(cx + o, cy + o);
        break;
    case 4:
        pip(cx - o, cy - o);
        pip(cx + o, cy - o);
        pip(cx - o, cy + o);
        pip(cx + o, cy + o);
        break;
    case 5:
        pip(cx, cy);
        pip(cx - o, cy - o);
        pip(cx + o, cy - o);
        pip(cx - o, cy + o);
        pip(cx + o, cy + o);
        break;
    case 6:
        pip(cx - o, cy - o);
        pip(cx,     cy - o);
        pip(cx + o, cy - o);
        pip(cx - o, cy + o);
        pip(cx,     cy + o);
        pip(cx + o, cy + o);
        break;
    }
}
