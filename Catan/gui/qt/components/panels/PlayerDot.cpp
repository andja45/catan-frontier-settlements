#include <components/panels/PlayerDot.h>
#include <QPainter>

void PlayerDot::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);
    p.setBrush(m_color);
    p.drawEllipse(rect().adjusted(1,1,-1,-1));
}
