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
    void setHighlighted(bool on) { m_highlighted = on; }
    bool hovered() const { return m_hovered; }
    bool highlighted() const { return m_highlighted; }

    void paint(QPainter& p, double size);

    static QVector<QPointF> hexPolygonPoints(const QPointF& center, double size);

private:
    Tile*     m_tile = nullptr;
    QPolygonF m_poly;
    QPointF   m_center;
    QRectF    m_tokenRect;
    bool      m_hovered = false;
    bool      m_highlighted = false;
};

#endif // QTILE_H
