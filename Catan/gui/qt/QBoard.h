#ifndef QBOARD_H
#define QBOARD_H

#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QHash>
#include <QPolygonF>
#include <vector>

#include <board/Board.h>
#include <types/TypeAliases.h>

#include "QTile.h"

class QBoard : public QWidget {
    Q_OBJECT
public:
    explicit QBoard(QWidget* parent = nullptr, Board* board = nullptr);
    Board* getBoard() { return m_board; }
    void setPlacingRobber(bool on) { m_placingRobber = on; update(); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void leaveEvent(QEvent* e) override;

private:
    Board* m_board;
    double m_size = 40.0;       // hex radius (corner distance); will be auto-scaled

    Tile* m_hoveredTile = nullptr;
    QTile* m_hovered = nullptr;
    std::vector<QTile> m_qtiles;


    bool m_placingRobber = true;

    static QPointF axialToPixelPointy(const TileCoords& a, double size);
    static QVector<QPointF> hexPolygonPointy(const QPointF& center, double size);

    QRectF boundsForLayout(double size) const;
};

#endif // QBOARD_H
