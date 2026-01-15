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
    for (const auto& tile : m_board->getTiles())
        m_qtiles.emplace_back(tile.get());

    m_qnodes.clear();
    m_qnodes.reserve(m_board->getNodes().size());
    for(const auto& node : m_board->getNodes())
        m_qnodes.emplace_back(node.get());
}

QPointF QBoard::axialToPixelTile(const TileCoords& a, double size) {
    // hexcoord -> pixel
    // x = size * sqrt(3) * (q + r/2)
    // y = size * 3/2 * r
    const double x = size * SQRT3 * (static_cast<double>(a.q()) + static_cast<double>(a.r()) / 2.0);
    const double y = size * 1.5 * static_cast<double>(a.r());
    return {x, y};
}

QPointF QBoard::axialToPixelNode(const NodeCoords& a, double size){
    auto [tileX, tileY] = axialToPixelTile(a.axialCoords(), size);

    int anglePer60 = static_cast<int>(a.direction());
    double angle_radians = M_PI / 2 - anglePer60 * M_PI / 3;
    int offsetX = size * std::cos(angle_radians);
    int offsetY = size * std::sin(angle_radians);

    return QPointF(tileX + offsetX, tileY + offsetY);
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
        const QPointF c = axialToPixelTile(h->getTileCoord(), size);
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
        Tile* h = qt.tile();

        const QPointF center = axialToPixelTile(h->getTileCoord(), size) + offset;

        const auto pts = hexPolygonPointy(center, size);
        QPolygonF poly;
        poly.reserve(6);
        for (const auto& pt : pts) poly << pt;

        qt.updateGeometry(center, poly, size);

        // outline pen is owned by board, applied once
        p.setPen(pen);
        qt.paint(p, size, m_placingRobber);
    }

    for (auto& qn : m_qnodes) {
        Node* n = qn.node();

        const QPointF center = axialToPixelNode(n->getCoords(), size) + offset;

        qn.updateGeometry(center, size);

        // outline pen is owned by board, applied once
        p.setPen(pen);
        qn.paint(p, size);
    }
}

void QBoard::mouseMoveEvent(QMouseEvent* e) {
    const QPointF pos = e->position();

    // --- Tile hover only if placing robber ---
    QTile* hitTile = nullptr;
    if (m_placingRobber) {
        for (auto& qt : m_qtiles) {
            if (qt.contains(pos)) { hitTile = &qt; break; }
        }
    }

    // --- Node hover always (or gate it behind a future "placing building" flag) ---
    QNode* hitNode = nullptr;
    for (auto& qn : m_qnodes) {
        if (qn.contains(pos)) { hitNode = &qn; break; }
    }

    if (hitTile == m_hoveredQTile && hitNode == m_hoveredQNode) return;

    if (m_hoveredQTile) m_hoveredQTile->setHovered(false);
    m_hoveredQTile = hitTile;
    if (m_hoveredQTile) m_hoveredQTile->setHovered(true);

    if (m_hoveredQNode) m_hoveredQNode->setHovered(false);
    m_hoveredQNode = hitNode;
    if (m_hoveredQNode) m_hoveredQNode->setHovered(true);

    update();
}

void QBoard::mousePressEvent(QMouseEvent* e) {
    if (e->button() != Qt::LeftButton) return;

    PlayerId player = 1; // TODO: wire to your current player

    // If user clicked a node, build/upgrade there
    if (m_hoveredQNode) {
        if (m_hoveredQNode->handleClick(player)) {
            update();
            return;
        }
    }

    // Robber placement (only if that mode is enabled)
    if (m_placingRobber && m_hoveredQTile) {
        Tile* tile = m_hoveredQTile->tile();
        // m_board->placeRobber(tile->getId()) etc.
        update();
        return;
    }
}

void QBoard::leaveEvent(QEvent* e) {
    Q_UNUSED(e);
}
