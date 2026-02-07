#ifndef PLAYERDOT_H
#define PLAYERDOT_H

#include <QColor>
#include <QHBoxLayout>
#include <QLabel>
#include <string>
#include <common/player/PlayerDot.h>
#include <screens/game/toolbar/buttons/DiceWidget.h>
#include <QStackedLayout>
#include <sstream>
#include <player/Player.h>

class PlayerDot : public QWidget {
public:
    explicit PlayerDot(QColor col,QWidget* parent = nullptr) : QWidget(parent) {
        setColor(col);
        setFixedSize(34, 34);
    }
    void setColor(const QColor& c) { m_color = c; update(); }

protected:
    void paintEvent(QPaintEvent*) override;
    QSize sizeHint() const override;
private:
    QColor m_color = QColor(255, 140, 60);
};

class DotAndText : public QWidget{
public:
    explicit DotAndText(const QColor& col, const QString& name, QWidget* parent = nullptr);
    static DotAndText* fromPlayer(Player*, QWidget* parent = nullptr);
private:
    QColor m_color;
    PlayerDot* m_dot;
    QLabel* m_label;
};


class TextInDot: public QWidget  {
    Q_OBJECT
public:
    TextInDot(const QColor &col, const QColor &textCol, const QString &name, QWidget *parent);

    void setText(const QString& text);
    static TextInDot* fromPlayer(Player*, QWidget* parent = nullptr);
    static std::string getInitials(const std::string &name) ;

private:
    PlayerDot* m_dot;
    QLabel* m_label;

};






#endif // PLAYERDOT_H
