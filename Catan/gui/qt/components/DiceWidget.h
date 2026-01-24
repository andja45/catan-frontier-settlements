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

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawDie(QPainter& p, const QRectF& rect, int value);

    int m_die1 = 1;
    int m_die2 = 1;
};
#endif // DICEWIDGET_H
