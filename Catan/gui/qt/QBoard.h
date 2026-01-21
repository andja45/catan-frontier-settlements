#ifndef QBOARD_H
#define QBOARD_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <vector>
#include <set>

#include <board/Board.h>
#include <types/TypeAliases.h>

#include <QTile.h>
#include <QNode.h>
#include <QEdge.h>

class QBoard : public QWidget {
    Q_OBJECT
public:
    explicit QBoard(QWidget* parent = nullptr, Board* board = nullptr);
    Board* getBoard() { return m_board; }
    void setPlacingRobber(bool on) { m_placingRobber = on; update(); }
    void setHighlightedEdges (const std::set<Edge*>& highlightedEdges);
    void setHighlightedNodes (const std::set<Node*>& highlightedNodes);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void leaveEvent(QEvent* e) override;

private:
    Board* m_board;
    double m_size = 40.0;       // hex radius (corner distance); will be auto-scaled

    QTile* m_hoveredQTile = nullptr;
    std::vector<QTile> m_qtiles;
    bool m_placingRobber = true;

    QNode* m_hoveredQNode = nullptr;
    std::vector<QNode> m_qnodes;

    QEdge* m_hoveredQEdge = nullptr;
    std::vector<QEdge> m_qedges;

    static QPointF axialToPixelTile(const TileCoords& a, double size);

    static QPointF axialToPixelNode(const NodeCoords& a, double size);

    QRectF boundsForLayout(double size) const;
};

#endif // QBOARD_H
