#ifndef COUNTBADGE_H
#define COUNTBADGE_H
#include<QWidget>
#include<QPainter>
#include<QFont>
#include<QLabel>
class CountBadge : public QLabel
{
private:
    int m_count=-1;
public:
    explicit CountBadge(int count, QWidget* parent);
    void incrementCount(int add){addToCount(1);}
    void decrementCount(int add){sudtractFromCount(1);}
    void addToCount(int add){m_count+=add;refresh();}
    void sudtractFromCount(int sub){m_count-=sub;refresh();}
    void paint(QPainter* painter, const QRectF& rect) const;
    void refresh(){setText(QString::number(m_count));}
};
/*
class CountBadge
{
private:
    int m_count=-1;
public:
    explicit CountBadge(int count)
        : m_count(count)
    {}
    void incrementCount(int add){addToCount(1);}
    void decrementCount(int add){sudtractFromCount(1);}
    void addToCount(int add){m_count+=add;}
    void sudtractFromCount(int sub){m_count-=sub;}
    void paint(QPainter* painter, const QRectF& rect) const;
};

*/
/*class CountBadge
{Q_OBJECT
private:
    int m_count=-1;
    QPainter *m_painter;
    QWidget* m_parent;
    void refresh();
protected:
    void paintEvent(QPaintEvent* event) override;
public:
    explicit CountBadge(int count, QWidget* parent=nullptr);
    void incrementCount(int add){addToCount(1);refresh();}
    void decrementCount(int add){sudtractFromCount(1); refresh();}
    void addToCount(int add){m_count+=add;refresh();}
    void sudtractFromCount(int sub){m_count-=sub;refresh();}
};
*/
#endif // COUNTBADGE_H
