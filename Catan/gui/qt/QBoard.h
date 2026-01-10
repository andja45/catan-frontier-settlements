#ifndef QBOARD_H
#define QBOARD_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QHash>
#include <QPolygonF>

#include <board/Board.h>
#include <types/TypeAliases.h>

class QBoard : public QWidget {
    Q_OBJECT
public:
    explicit QBoard(QWidget* parent = nullptr, Board* board = nullptr);
    Board* getBoard() { return m_board; }
    void setPlacingRobber(bool on) { m_placingRobber = on; update(); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void leaveEvent(QEvent* e) override;

private:
    Board* m_board;
    //QVector<HexCoords> m_hexes;     // 19 land hexes
    double m_size = 40.0;       // hex radius (corner distance); will be auto-scaled

    // Hover state (store coord or pointer)
    Tile* m_hoveredTile = nullptr;

    // Cache polygons from last paint for hit-testing
    QHash<Tile*, QPolygonF> m_tilePoly;
    QHash<Node*, QPolygonF> m_nodePoly;

    bool m_placingRobber = true;

    static QPointF axialToPixelPointy(const TileCoords& a, double size);
    static QVector<QPointF> hexPolygonPointy(const QPointF& center, double size);

    QRectF boundsForLayout(double size) const;
    void paintTile(Tile* h, const double size, const QPointF offset, QPainter& p, QPen& pen);
};

#endif // QBOARD_H
