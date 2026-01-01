#include "catanhexwidget.h"

#include <QPainter>
#include <QtMath>
#include <algorithm>

static constexpr double SQRT3 = 1.7320508075688772;

CatanHexWidget::CatanHexWidget(QWidget *parent) : QWidget(parent) {
    // Standard Catan land layout is a hexagon of radius 2 in hex-grid terms (total 19 tiles).
    // Axial coords (q, r) for hexagon radius 2:
    // all (q, r) with |q|<=2, |r|<=2, |q+r|<=2
    for (int q = -2; q <= 2; ++q) {
        for (int r = -2; r <= 2; ++r) {
            int s = -q - r;
            if (std::abs(q) <= 2 && std::abs(r) <= 2 && std::abs(s) <= 2) {
                m_hexes.push_back({q, r});
            }
        }
    }

    setMinimumSize(300, 300);
    setAutoFillBackground(true);
}

QPointF CatanHexWidget::axialToPixelPointy(const Axial& a, double size) {
    // Pointy-top axial -> pixel
    // x = size * sqrt(3) * (q + r/2)
    // y = size * 3/2 * r
    const double x = size * SQRT3 * (static_cast<double>(a.q) + static_cast<double>(a.r) / 2.0);
    const double y = size * 1.5 * static_cast<double>(a.r);
    return {x, y};
}

QVector<QPointF> CatanHexWidget::hexPolygonPointy(const QPointF& center, double size) {
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

QRectF CatanHexWidget::boundsForLayout(double size) const {
    // Compute bounding box in "layout space" for given size (before centering in widget)
    bool first = true;
    double minX = 0, minY = 0, maxX = 0, maxY = 0;

    for (const auto& h : m_hexes) {
        const QPointF c = axialToPixelPointy(h, size);
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

void CatanHexWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Background
    p.fillRect(rect(), palette().window());

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

    for (const auto& h : m_hexes) {
        const QPointF center = axialToPixelPointy(h, size) + offset;
        const auto pts = hexPolygonPointy(center, size);

        QPolygonF poly;
        poly.reserve(6);
        for (const auto& pt : pts) poly << pt;

        p.drawPolygon(poly);
    }
}
