#ifndef CATANHEXWIDGET_H
#define CATANHEXWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPointF>

#include "headers/Board/Board.h"
#include "headers/Types/TypeAliases.h"

class CatanHexWidget : public QWidget {
    Q_OBJECT
public:
    explicit CatanHexWidget(Board* board, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Board* m_board;
    //QVector<HexCoords> m_hexes;     // 19 land hexes
    double m_size = 40.0;       // hex radius (corner distance); will be auto-scaled

    static QPointF axialToPixelPointy(const HexCoords& a, double size);
    static QVector<QPointF> hexPolygonPointy(const QPointF& center, double size);

    QRectF boundsForLayout(double size) const;
};

#endif // CATANHEXWIDGET_H
