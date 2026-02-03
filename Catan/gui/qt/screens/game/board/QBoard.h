#ifndef QBOARD_H
#define QBOARD_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <vector>
#include <set>

#include <board/Board.h>
#include <types/TypeAliases.h>

#include "elements/QTile.h"
#include "elements/QNode.h"
#include "elements/QEdge.h"

#include "renderstate/BoardRenderState.h"

class QBoard : public QWidget {
    Q_OBJECT
public:
    explicit QBoard(QWidget* parent = nullptr, Board* board = nullptr);
    Board* getBoard() { return m_board; }

    void setHighlightedEdges (const std::vector<EdgeId>& highlightedEdges);
    void setHighlightedNodes (const std::vector<NodeId>& highlightedNodes);
    void setHighlightedTiles (const std::vector<TileId>& highlightedTiles);

    void clearHighlights();
// TODO controller connect here <<<
public slots:
    void update(const BoardRenderState& renderState);
    void onPlayBuildFeedback();

signals:
    void tileClicked(TileId tileId);
    void nodeClicked(NodeId nodeId);
    void edgeClicked(EdgeId edgeId);
    void elementClicked(int id);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void leaveEvent(QEvent* e) override;

private:
    Board* m_board;
    double m_size = 40.0;       // hex radius (corner distance); will be auto-scaled
    PulseState* m_pulse = nullptr;

    QTile* m_hoveredQTile = nullptr;
    std::vector<QTile> m_qtiles;
    bool m_placingRobber = true;

    QNode* m_hoveredQNode = nullptr;
    std::vector<QNode> m_qnodes;

    QEdge* m_hoveredQEdge = nullptr;
    std::vector<QEdge> m_qedges;

    double m_shakeX = 0.0;
    double m_shakeY = 0.0;

    static QPointF axialToPixelTile(const TileCoords& a, double size);

    static QPointF axialToPixelNode(const NodeCoords& a, double size);

    QRectF boundsForLayout(double size) const;

};

#endif // QBOARD_H
