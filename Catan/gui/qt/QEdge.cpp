#include <QEdge.h>
#include <cmath>
#include <GameTheme.h>

bool QEdge::hoverAllowed() const {
    if (!m_edge) return false;
    return !m_edge->isOccupied();
}

QColor QEdge::playerColor() const {
    return QColor(40, 40, 40);
}

void QEdge::updateGeometry(const QPointF& p, const QPointF& q, double size) {
    m_p = p;
    m_q = q;
    m_center = (m_p + m_q) / 2;
    const double r = size / 4.5;
    m_hit = QPainterPath();
    m_hit.addEllipse(m_center, r, r);
}

bool QEdge::contains(const QPointF& p) const {
    return m_hit.contains(p);
}

bool QEdge::handleClick(PlayerId player) {
    if (!m_edge) return false;

    if (!m_edge->isOccupied()) {
        m_edge->setOwner(player);
        return true;
    }
    return false; // already road -> nothing
}

void QEdge::drawRoad(QPainter& p, double size) {
    const double w = 0.15; // width
    auto [x, y] = m_q - m_p;
    QPointF ort = w/2 * QPointF(-y, x); //orthogonal vector

    QPolygonF quad;
    quad << m_p + ort << m_p - ort << m_q - ort << m_q + ort;

    p.save();
    p.setBrush(playerColor());
    p.setPen(QPen(Qt::black, 1.5));
    p.drawPolygon(quad);
    p.restore();
}
void QEdge::drawPort(QPainter& p, double size) {
    auto [r, s] = equilateralThirdVertices(m_q, m_p);
    const double w = 0.2; // width
    const double l = 0.4; // length
    auto color = QColor::fromRgb(200, 160, 10);
    p.save();
    p.setBrush(color);
    p.setPen(QPen(color, 1.5));

    {
        auto [x, y] = r - m_p;
        QPointF ort = w/2 * QPointF(-y, x); //orthogonal vector

        QPolygonF quad;
        quad << m_p + ort << m_p - ort << r - ort - l * (r - m_p) << r + ort - l * (r - m_p);

        p.drawPolygon(quad);

    }
    {
        auto [x, y] = r - m_q;
        QPointF ort = w/2 * QPointF(-y, x); //orthogonal vector

        QPolygonF quad;
        quad << m_q + ort << m_q - ort << r - ort - l * (r - m_q) << r + ort - l * (r - m_q);

        p.drawPolygon(quad);
    }

    if(m_edge->getTradeType() == TradeType::None){
        p.setBrush(Qt::white);
    }

    else{
        p.setBrush(GameTheme::getColorByResource(m_edge->getTradeType()));
    }
    p.setPen(QPen(Qt::black, 1.5));

    p.drawEllipse(r, size / 4, size / 4);
    p.restore();
}


void QEdge::paint(QPainter& p, double size) {
    if (!m_edge) return;

    // Hover ring: only over empty or settlement edges
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

    // Draw road if present
    if (m_edge->isOccupied()) {
        drawRoad(p, size);
    }

    // Draw port if present
    if (m_edge->hasTrade()) {
        drawPort(p, size);
    }
}

QPointF QEdge::rotate(const QPointF& v, double angleRad) {
    const double c = std::cos(angleRad);
    const double s = std::sin(angleRad);
    return QPointF(
        v.x() * c - v.y() * s,
        v.x() * s + v.y() * c
    );
}

std::pair<QPointF, QPointF> QEdge::equilateralThirdVertices(const QPointF& A, const QPointF& B)
{
    const QPointF v = B - A;

    constexpr double angle = M_PI / 3.0; // 60°

    QPointF C1 = A + rotate(v,  angle);  // left side
    QPointF C2 = A + rotate(v, -angle);  // right side

    return {C1, C2};
}
