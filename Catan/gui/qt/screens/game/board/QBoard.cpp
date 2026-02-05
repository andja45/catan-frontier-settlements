#include "QBoard.h"

#include "../../../common/audio/AudioManager.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <QElapsedTimer>
#include <QEasingCurve>
#include <cmath>

static constexpr double SQRT3 = 1.7320508075688772;

QBoard::QBoard(QWidget *parent, Board* board) : QWidget(parent), m_board(board) {
    setMinimumSize(300, 300);
    setAutoFillBackground(true);
    setMouseTracking(true);

    m_pulse=new PulseState(this);
    connect(m_pulse, &PulseState::changed, this, QOverload<>::of(&QWidget::update));

    m_qtiles.clear();
    m_qtiles.reserve(m_board->getTiles().size());
    for (const auto& tile : m_board->getTiles()) {
        m_qtiles.emplace_back(tile.get());
        m_qtiles.back().setPulse(m_pulse);
    }

    m_qnodes.clear();
    m_qnodes.reserve(m_board->getNodes().size());
    for(const auto& node : m_board->getNodes()) {
        m_qnodes.emplace_back(node.get());
        m_qnodes.back().setPulse(m_pulse);
    }

    m_qedges.clear();
    m_qedges.reserve(m_board->getEdges().size());
    for(const auto& edge : m_board->getEdges()) {
        m_qedges.emplace_back(edge.get());
        m_qedges.back().setPulse(m_pulse);
    }
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
    double angle_radians = -M_PI / 2 + anglePer60 * M_PI / 3;
    double offsetX = size * std::cos(angle_radians);
    double offsetY = size * std::sin(angle_radians);

    return QPointF(tileX + offsetX, tileY + offsetY);
}

QRectF QBoard::boundsForLayout(double size) const {
    // Compute bounding box in "layout space" for given size (before centering in widget)
    bool first = true;
    double minX = 0, minY = 0, maxX = 0, maxY = 0;

    for (const auto& h : m_board->getTiles()) {
        const QPointF c = axialToPixelTile(h->getTileCoord(), size);
        const auto poly = QTile::hexPolygonPoints(c, size);
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
    return QRectF(QPointF(minX - size, minY - size), QPointF(maxX + size, maxY + size));
}

void QBoard::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Background
    p.fillRect(rect(), QColor(80, 140, 200));

    p.save();
    p.translate(m_shakeX, m_shakeY);

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

        qt.updateGeometry(center, size);

        // outline pen is owned by board, applied once

        p.setPen(pen);
        qt.paint(p, size);
    }

    for (auto& qe : m_qedges) {
        Edge* e = qe.edge();

        const QPointF start = axialToPixelNode(e->getStart()->getCoords(), size) + offset;
        const QPointF end = axialToPixelNode(e->getEnd()->getCoords(), size) + offset;

        qe.updateGeometry(start, end, size);

        // outline pen is owned by board, applied once
        p.setPen(pen);
        qe.paint(p, size);
    }

    for (auto& qn : m_qnodes) {
        Node* n = qn.node();

        const QPointF center = axialToPixelNode(n->getCoords(), size) + offset;

        qn.updateGeometry(center, size);

        // outline pen is owned by board, applied once
        p.setPen(pen);
        qn.paint(p, size);
    }

    p.restore();
}

void QBoard::mouseMoveEvent(QMouseEvent* e) {
    const QPointF pos = e->position();

    QTile* hitTile = nullptr;
    for (auto& qt : m_qtiles) {
        if (qt.contains(pos)) { hitTile = &qt; break; }
    }

    QNode* hitNode = nullptr;
    for (auto& qn : m_qnodes) {
        if (qn.contains(pos)) { hitNode = &qn; break; }
    }

    QEdge* hitEdge = nullptr;
    for (auto& qe : m_qedges) {
        if (qe.contains(pos)) { hitEdge = &qe; break; }
    }


    if (m_hoveredQTile) m_hoveredQTile->setHovered(false);
    m_hoveredQTile = hitTile;
    if (m_hoveredQTile) m_hoveredQTile->setHovered(true);

    if (m_hoveredQNode) m_hoveredQNode->setHovered(false);
    m_hoveredQNode = hitNode;
    if (m_hoveredQNode) m_hoveredQNode->setHovered(true);

    if (m_hoveredQEdge) m_hoveredQEdge->setHovered(false);
    m_hoveredQEdge = hitEdge;
    if (m_hoveredQEdge) m_hoveredQEdge->setHovered(true);

    QWidget::update();
}

void QBoard::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() != Qt::LeftButton) return;


    if (m_hoveredQNode) {
        emit nodeClicked(m_hoveredQNode->node()->getNodeId());
        emit elementClicked(m_hoveredQNode->node()->getNodeId());
    }

    else if (m_hoveredQEdge) {
        emit edgeClicked(m_hoveredQEdge->edge()->getEdgeId());
        emit elementClicked(m_hoveredQEdge->edge()->getEdgeId());
    }

    else if (m_hoveredQTile) {
        emit tileClicked(m_hoveredQTile->tile()->getTileId());
        emit elementClicked(m_hoveredQTile->tile()->getTileId());
    }
}

void QBoard::leaveEvent(QEvent* e) {
    Q_UNUSED(e);
}

void QBoard::setHighlightedEdges (const std::vector<EdgeId>& highlightedEdges) {
    for(int id : highlightedEdges) {
        m_qedges[id].setHighlighted(true);
    }
}

void QBoard::setHighlightedNodes (const std::vector<NodeId>& highlightedNodes) {
    for(int id: highlightedNodes) {
        m_qnodes[id].setHighlighted(true);
    }
}

void QBoard::setHighlightedTiles(const std::vector<TileId> &highlightedTiles) {
    for (int id:highlightedTiles) {
        m_qtiles[id].setHighlighted(true);
    }
}

void QBoard::clearHighlights() {
    for (auto& qt : m_qtiles) qt.setHighlighted(false);
    for (auto& qn : m_qnodes) qn.setHighlighted(false);
    for (auto& qe : m_qedges) qe.setHighlighted(false);

}

void QBoard::update(const BoardRenderState &renderState) {
    clearHighlights();

    setHighlightedTiles(renderState.getHighlightedTiles());
    setHighlightedNodes(renderState.getHighlightedNodes());
    setHighlightedEdges(renderState.getHighlightedEdges());

    QWidget::update();
}


void QBoard::onPlayBuildFeedback() {
    AudioManager::instance().playBuild();

    const int durationMs = 850;
    const double ampPx   = 6.5;
    const double freqHz  = 20.0;

    m_shakeX = 0.0;
    m_shakeY = 0.0;

    auto* timer = new QTimer(this);
    timer->setInterval(8);

    auto* clock = new QElapsedTimer();
    clock->start();

    QEasingCurve ease(QEasingCurve::OutExpo);

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        const double t = clock->elapsed() / double(durationMs);
        if (t >= 1.0) {
            m_shakeX = 0.0;
            m_shakeY = 0.0;
            timer->stop();
            timer->deleteLater();
            delete clock;
            QWidget::update();
            return;
        }

        const double strength = 1.0 - ease.valueForProgress(t);
        const double a = ampPx * strength;

        const double w1 = 2.0 * M_PI * freqHz;
        const double w2 = 2.0 * M_PI * (freqHz * 1.7);

        const double noiseX = (QRandomGenerator::global()->generateDouble() - 0.5) * 0.6;
        const double noiseY = (QRandomGenerator::global()->generateDouble() - 0.5) * 0.6;

        m_shakeX =
            a * (0.7 * std::sin(w1 * t)
               + 0.3 * std::sin(w2 * t + 1.3))
            + noiseX * strength;

        m_shakeY =
            a * (0.6 * std::cos(w1 * t * 0.9)
               + 0.4 * std::sin(w2 * t + 2.1))
            + noiseY * strength;

        QWidget::update();
    });

    timer->start();
}
