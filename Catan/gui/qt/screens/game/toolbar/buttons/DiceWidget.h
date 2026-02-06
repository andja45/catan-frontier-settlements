#ifndef DICEWIDGET_H
#define DICEWIDGET_H
#include <QWidget>
#include <QPainter>
#include <screens/game/board/elements/PulseState.hpp>

class DiceWidget : public QWidget {
    Q_OBJECT
public:
    explicit DiceWidget(const std::pair<int,int>*dice,QWidget* parent = nullptr);


    QSize sizeHint() const override { return {120, 60}; }
    void setEnabled(bool enabled=true);
    void setHighlighted(bool highlight);

signals:
    void clicked();
    void dieClicked(int index);
public slots:
    void redraw();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* e) override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void leaveEvent(QEvent* e) override;
private:
    void drawDie(QPainter& p, const QRectF& rect, int value);
    void updateDieRects();

    const std::pair<int, int>* m_diceValues;

    QRectF m_die1Rect;
    QRectF m_die2Rect;

    int m_hoveredDie = 0; // 0 none, 1 die1, 2 die2
    bool m_disabled=false;
    bool m_highlight=false;
    PulseState* m_pulse = nullptr;

};
#endif // DICEWIDGET_H
