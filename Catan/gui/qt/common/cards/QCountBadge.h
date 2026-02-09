#ifndef COUNTBADGE_H
#define COUNTBADGE_H
#include<QWidget>
#include<QPainter>
#include<QFont>
#include<QLabel>
#include <common/theme/GameTheme.h>

class QCountBadge : public QLabel
{
private:
    int m_count=-1;
    QColor m_color=Qt::black;
public:
    void setColor(QColor col);

    void setGolden(bool golden) {
        if (golden) {
            setColor(GameTheme::getGoldenColor());
        }
        else {
            setColor(Qt::black);
        }
    }
    explicit QCountBadge(int count, QWidget* parent);
    void incrementCount(int add){addToCount(1);}
    void decrementCount(int add){subtractFromCount(1);}
    void addToCount(int add){m_count+=add;refresh();}
    void subtractFromCount(int sub){m_count-=sub;refresh();}
    void refresh(){setText(QString::number(m_count));}
};
#endif // COUNTBADGE_H
