#ifndef QEDGE_H
#define QEDGE_H

#include <QPolygonF>
#include <QPointF>
#include <QPainter>
#include <QPainterPath>

#include <board/Edge.h>
#include <screens/game/board/elements/PulseState.hpp>

class QEdge {
public:
    bool highlighted = true;

    explicit QEdge(Edge* edge = nullptr) : m_edge(edge) {}
    void setPulse(PulseState* pulse) { m_pulse = pulse; }

    Edge* edge() const { return m_edge; }

    void updateGeometry(const QPointF& p, const QPointF& q, double size);
    bool contains(const QPointF& p) const;

    void setHovered(bool on) { m_hovered = on; }
    bool hovered() const { return m_hovered; }

    // draw hover + settlement/city
    void paint(QPainter& p, double size);

    static QPointF rotate(const QPointF &v, double angleRad);
    static std::pair<QPointF, QPointF> equilateralThirdVertices(const QPointF &A, const QPointF &B);

    void setHighlighted(bool on) { m_highlighted = on; }
    bool isHighlighted() const { return m_highlighted; }


private:
    bool        m_highlighted = false;

    bool hoverAllowed() const; // empty or road
    void drawRoad(QPainter& p, double size);
    void drawPort(QPainter& p, double size);
    QColor playerColor() const; // placeholder

private:
    Edge*       m_edge = nullptr;  // non-owning
    QPointF     m_p, m_q, m_center;
    QPainterPath   m_hit;            // for hit-test
    bool        m_hovered = false;
    PulseState* m_pulse = nullptr;
};

#endif // QEDGE_H
