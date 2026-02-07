#include "DiceWidget.h"

#include "../../../../common/audio/AudioManager.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <common/theme/GameTheme.h>

DiceWidget::DiceWidget(const std::pair<int,int>*dice,QWidget* parent)
    : QWidget(parent), m_diceValues(dice)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setCursor(Qt::PointingHandCursor);
    setMouseTracking(true);

    setMinimumSize(QSize(110, 50));
    m_pulse = new PulseState(this);
    connect(m_pulse, &PulseState::changed, this, [this]() {
        update();
    });
}

void DiceWidget::updateDieRects()
{
    const int spacing = 10;
    const int size = qMin(height(), (width() - spacing) / 2)-4;

    m_die1Rect = QRectF(2, (height() - size) / 2, size, size);
    m_die2Rect = QRectF(2+size + spacing, (height() - size) / 2, size, size);
}

void DiceWidget::redraw() {
    QWidget::update();
}

void DiceWidget::setHighlighted(bool highlight) {
    m_highlight=highlight;
}

void DiceWidget::paintEvent(QPaintEvent*)
{
    updateDieRects();

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    drawDie(p, m_die1Rect, m_diceValues->first);
    drawDie(p, m_die2Rect, m_diceValues->second);

    // Hover overlay
    if (!m_disabled && m_hoveredDie != 0) {
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
        if (!m_disabled) {
            AudioManager::instance().playDiceRll();
            emit clicked();
        }

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
    auto color=QColor(250, 250, 250);
    if (m_disabled)
        color=color.darker(120);

    p.setBrush(color);
    p.setPen(QColor(0, 0, 0, 120));
    p.drawRoundedRect(r, 8, 8);

    if (m_highlight) {
        auto rGrow=r.adjusted(-1,-1,1,1);
        p.setBrush(Qt::NoBrush);
        auto col=m_pulse->pulseColor(GameTheme::getGoldenColor());
        p.setPen(QPen(col,4));
        p.drawRoundedRect(rGrow, 8, 8);
    }

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
        pip(cx - o, cy - o); pip(cx-o, cy ); pip(cx - o, cy + o);
        pip(cx + o, cy - o); pip(cx+o, cy ); pip(cx + o, cy + o);
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

void DiceWidget::setEnabled(bool enabled) {
    m_disabled=!enabled;
    setHighlighted(enabled);
    update();
}
