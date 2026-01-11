#include "QBoard.h"

#include <QPainter>
#include <QtMath>
#include <QMouseEvent>
#include <cmath>

static constexpr double SQRT3 = 1.7320508075688772;

QBoard::QBoard(QWidget *parent, Board* board) : QWidget(parent), m_board(board) {
    setMinimumSize(300, 300);
    setAutoFillBackground(true);
    setMouseTracking(true);

    m_qtiles.clear();
    m_qtiles.reserve(m_board->getTiles().size());
    for (const auto& up : m_board->getTiles())
        m_qtiles.emplace_back(up.get());
}

QPointF QBoard::axialToPixelPointy(const TileCoords& a, double size) {
    // hexcoord -> pixel
    // x = size * sqrt(3) * (q + r/2)
    // y = size * 3/2 * r
    const double x = size * SQRT3 * (static_cast<double>(a.q()) + static_cast<double>(a.r()) / 2.0);
    const double y = size * 1.5 * static_cast<double>(a.r());
    return {x, y};
}

QVector<QPointF> QBoard::hexPolygonPointy(const QPointF& center, double size) {
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

QRectF QBoard::boundsForLayout(double size) const {
    // Compute bounding box in "layout space" for given size (before centering in widget)
    bool first = true;
    double minX = 0, minY = 0, maxX = 0, maxY = 0;

    for (const auto& h : m_board->getTiles()) {
        const QPointF c = axialToPixelPointy(h->getTileCoord(), size);
        const auto poly = hexPolygonPointy(c, size);
        for (const auto& p : poly) {
            if (first) {
                minX = maxX = p.x();
                minY = maxY = p.y();
                first = false;
            } else {
                minX = std::min(minX, p.x());
                minY = std::min(minY, p.y());
                maxX = std::max(maxX, p.x());
                maxY = std::max(maxY, p.y());
            }
        }
    }
    return QRectF(QPointF(minX, minY), QPointF(maxX, maxY));
}

void QBoard::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);


    // Background
    p.fillRect(rect(), QColor(80, 140, 200));

    // Auto-scale hex size to fit with margins
    const double margin = 20.0;
    const QSizeF avail = QSizeF(width() - 2 * margin, height() - 2 * margin);

    // Find size that fits: do a quick 1D scale based on bounds at size=1
    const QRectF unitBounds = boundsForLayout(1.0);
    const double unitW = unitBounds.width();
    const double unitH = unitBounds.height();

    // size * unitW <= availW and size * unitH <= availH
    const double sizeByW = avail.width() / unitW;
    const double sizeByH = avail.height() / unitH;
    const double size = std::max(5.0, std::min(sizeByW, sizeByH));

    const QRectF b = boundsForLayout(size);

    // Center in widget
    const QPointF offset(
        margin + (avail.width()  - b.width())  / 2.0 - b.left(),
        margin + (avail.height() - b.height()) / 2.0 - b.top()
    );

    // Pen for outlines
    QPen pen(Qt::black);
    pen.setWidthF(2.0);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);

    for (auto& qt : m_qtiles) {
        Tile* h = qt.model();

        const QPointF center = axialToPixelPointy(h->getTileCoord(), size) + offset;

        const auto pts = hexPolygonPointy(center, size);
        QPolygonF poly;
        poly.reserve(6);
        for (const auto& pt : pts) poly << pt;

        qt.updateGeometry(center, poly, size);

        // outline pen is owned by board, applied once
        p.setPen(pen);
        qt.paint(p, size, m_placingRobber);
    }

}

void QBoard::mouseMoveEvent(QMouseEvent* e) {
    if (!m_placingRobber) return;

    const QPointF pos = e->position();

    QTile* hit = nullptr;
    for (auto& qt : m_qtiles) {
        if (qt.contains(pos)) { hit = &qt; break; }
    }

    if (hit == m_hovered) return;

    if (m_hovered) m_hovered->setHovered(false);
    m_hovered = hit;
    if (m_hovered) m_hovered->setHovered(true);

    update();
}

void QBoard::mousePressEvent(QMouseEvent* e) {
    if (!m_placingRobber || e->button() != Qt::LeftButton) return;
    if (!m_hovered) return;

    Tile* tile = m_hovered->model();
    // call model: m_board->placeRobber(tile->getId()) etc.
}

void QBoard::leaveEvent(QEvent* e) {
    Q_UNUSED(e);
    if (m_hoveredTile) {
        m_hoveredTile = nullptr;
        update();
    }
}
