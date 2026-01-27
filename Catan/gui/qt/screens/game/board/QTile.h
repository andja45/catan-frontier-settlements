#ifndef QTILE_H
#define QTILE_H

#include <QPolygonF>
#include <QPointF>
#include <QPainter>

#include <board/Tile.h>

class QTile {
public:
    explicit QTile(Tile* tile = nullptr) : m_tile(tile) {}

    Tile* tile() const { return m_tile; }

    void updateGeometry(const QPointF& center, double size);
    bool contains(const QPointF& p) const;

    void setHovered(bool on) { m_hovered = on; }
    bool hovered() const { return m_hovered; }

    void paint(QPainter& p, double size, bool placingRobber);

    static QVector<QPointF> hexPolygonPoints(const QPointF& center, double size);

private:
    Tile*     m_tile = nullptr;
    QPolygonF m_poly;
    QPointF   m_center;
    QRectF    m_tokenRect;
    bool      m_hovered = false;
};

#endif // QTILE_H
