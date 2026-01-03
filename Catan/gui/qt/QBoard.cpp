#include "QBoard.h"

#include <QPainter>
#include <QtMath>
#include <QMouseEvent>
#include <cmath>

static constexpr double SQRT3 = 1.7320508075688772;

QBoard::QBoard(QWidget *parent, Board* board) : QWidget(parent), m_board(board) {
    if(m_board == nullptr)
        m_board = new Board();
    setMinimumSize(300, 300);
    setAutoFillBackground(true);
    setMouseTracking(true);
}

QPointF QBoard::axialToPixelPointy(const HexCoords& a, double size) {
    // hexcoord -> pixel
    // x = size * sqrt(3) * (q + r/2)
    // y = size * 3/2 * r
    const double x = size * SQRT3 * (static_cast<double>(a.first) + static_cast<double>(a.second) / 2.0);
    const double y = size * 1.5 * static_cast<double>(a.second);
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

    for (const auto& h : m_board->getTiles()) {
        const QPointF center = axialToPixelPointy(h->getTileCoord(), size) + offset;
        const auto pts = hexPolygonPointy(center, size);

        QPolygonF poly;
        poly.reserve(6);
        for (const auto& pt : pts) poly << pt;

        m_tilePoly[h.get()] = poly;   // cache for hit test

        //color hex
        QBrush brush(Qt::NoBrush);

        switch (h->getType()) {
        case ResourceType::Wood:
            brush = QBrush(QColor(34, 139, 34));      // forest green
            break;
        case ResourceType::Brick:
            brush = QBrush(QColor(178, 74, 34));      // firebrick red
            break;
        case ResourceType::Ore:
            brush = QBrush(QColor(160, 160, 160));    // dim gray
            break;
        case ResourceType::Wool:
            brush = QBrush(QColor(144, 238, 144));    // light green
            break;
        case ResourceType::Wheat:
            brush = QBrush(QColor(230, 205, 22));     // goldenrod
            break;
        case ResourceType::Desert:
            brush = QBrush(QColor(200, 165, 112));     // sand
            break;
        case ResourceType::Sea:
            brush = QBrush(QColor(80, 140, 200));     // steel blue
            break;
        case ResourceType::None:
        default:
            brush = QBrush(Qt::NoBrush);
            break;
        }

        p.setBrush(brush);
        p.drawPolygon(poly);

        if (m_placingRobber && h.get() == m_hoveredTile) {
            // overlay highlight
            p.save();
            p.setBrush(QBrush(QColor(255, 255, 255, 60))); // translucent
            p.drawPolygon(poly);
            p.restore();
        }

        //draw circle and number
        if(h->getNumber() == 7) continue;
        p.setBrush(QColor(240, 240, 210));
        p.drawEllipse(center, size / 3, size / 3);

        //this formula makes numbers scale with screen size, there's no particular logic, I tried it out
        QFont font("Arial", size / 60 * (28 - 2.5 * std::abs(7 - h->getNumber()))); // 20 is the point size
        p.setFont(font);
        if(h->getNumber() == 6 || h->getNumber() == 8) {
            p.setPen(QColor(255, 0, 0));
            font.setWeight(QFont::Bold);
        }
        p.drawText(poly.boundingRect(), Qt::AlignCenter, QString::number(h->getNumber()));
        p.setPen(pen);
    }

}

void QBoard::mouseMoveEvent(QMouseEvent* e) {
    if (!m_placingRobber) {
        QWidget::mouseMoveEvent(e);
        return;
    }

    Tile* hit = nullptr;
    const QPointF pos = e->position();

    // Find which polygon contains the mouse
    for (auto it = m_tilePoly.constBegin(); it != m_tilePoly.constEnd(); ++it) {
        if (it.value().containsPoint(pos, Qt::OddEvenFill)) {
            hit = it.key();
            break;
        }
    }

    if (hit != m_hoveredTile) {
        m_hoveredTile = hit;
        update(); // trigger repaint to update highlight
    }
}

void QBoard::leaveEvent(QEvent* e) {
    Q_UNUSED(e);
    if (m_hoveredTile) {
        m_hoveredTile = nullptr;
        update();
    }
}
