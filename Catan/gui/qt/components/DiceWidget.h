#ifndef DICEWIDGET_H
#define DICEWIDGET_H
#include <QWidget>
#include <QPainter>
class DiceWidget : public QWidget {
    Q_OBJECT
public:
    explicit DiceWidget(QWidget* parent = nullptr);

    void setDice(int d1, int d2);

    QSize sizeHint() const override { return {120, 60}; }

    signals:
    void clicked();
    void dieClicked(int index);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* e) override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void leaveEvent(QEvent* e) override;

private:
    void drawDie(QPainter& p, const QRectF& rect, int value);
    void updateDieRects();

    int m_die1 = 1;
    int m_die2 = 1;

    QRectF m_die1Rect;
    QRectF m_die2Rect;

    int m_hoveredDie = 0; // 0 none, 1 die1, 2 die2
};
#endif // DICEWIDGET_H
