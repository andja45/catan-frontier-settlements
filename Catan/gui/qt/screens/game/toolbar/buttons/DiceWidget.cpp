#include "DiceWidget.h"

#include "../../../../common/audio/AudioManager.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

DiceWidget::DiceWidget(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setCursor(Qt::PointingHandCursor);
    setMouseTracking(true);
    setDice(1,1);
}

void DiceWidget::setDice(int d1, int d2)
{
    m_die1 = qBound(1, d1, 6);
    m_die2 = qBound(1, d2, 6);
    update();
}

void DiceWidget::updateDieRects()
{
    const int spacing = 10;
    const int size = qMin(height(), (width() - spacing) / 2);

    m_die1Rect = QRectF(0, (height() - size) / 2, size, size);
    m_die2Rect = QRectF(size + spacing, (height() - size) / 2, size, size);
}

void DiceWidget::paintEvent(QPaintEvent*)
{
    updateDieRects();

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    drawDie(p, m_die1Rect, m_die1);
    drawDie(p, m_die2Rect, m_die2);

    // Hover overlay
    if (m_hoveredDie != 0) {
        const QRectF& r = (m_hoveredDie == 1) ? m_die1Rect : m_die2Rect;

        p.save();
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(255, 255, 255, 60)); // translucent highlight
        p.drawRoundedRect(r, 8, 8);

        // border glow
        QPen glow(QColor(0, 0, 0, 60));
        glow.setWidthF(2.0);
        p.setBrush(Qt::NoBrush);
        p.setPen(glow);
        p.drawRoundedRect(r.adjusted(1,1,-1,-1), 8, 8);
        p.restore();
    }
}

void DiceWidget::mousePressEvent(QMouseEvent* e){
    if (e->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(e);
        return;
    }

    updateDieRects();

    const QPointF pos = e->position();

    // click anywhere on dice area triggers
    if (m_die1Rect.contains(pos) || m_die2Rect.contains(pos)) {
        AudioManager::instance().playDiceRll();
        emit clicked();

        // which die
        if (m_die1Rect.contains(pos)) emit dieClicked(1);
        else if (m_die2Rect.contains(pos)) emit dieClicked(2);

        e->accept();
        return;
    }

    QWidget::mousePressEvent(e);
}

void DiceWidget::drawDie(QPainter& p, const QRectF& r, int value)
{
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
    case 1: pip(cx, cy); break;
    case 2: pip(cx - o, cy - o); pip(cx + o, cy + o); break;
    case 3: pip(cx, cy); pip(cx - o, cy - o); pip(cx + o, cy + o); break;
    case 4: pip(cx - o, cy - o); pip(cx + o, cy - o); pip(cx - o, cy + o); pip(cx + o, cy + o); break;
    case 5: pip(cx, cy); pip(cx - o, cy - o); pip(cx + o, cy - o); pip(cx - o, cy + o); pip(cx + o, cy + o); break;
    case 6:
        pip(cx - o, cy - o); pip(cx, cy - o); pip(cx + o, cy - o);
        pip(cx - o, cy + o); pip(cx, cy + o); pip(cx + o, cy + o);
        break;
    }
}

void DiceWidget::mouseMoveEvent(QMouseEvent* e)
{
    updateDieRects();

    const QPointF pos = e->position();
    int newHover = 0;
    if (m_die1Rect.contains(pos)) newHover = 1;
    else if (m_die2Rect.contains(pos)) newHover = 2;

    if (newHover != m_hoveredDie) {
        m_hoveredDie = newHover;
        update();
    }

    QWidget::mouseMoveEvent(e);
}

void DiceWidget::leaveEvent(QEvent* e)
{
    if (m_hoveredDie != 0) {
        m_hoveredDie = 0;
        update();
    }
    QWidget::leaveEvent(e);
}