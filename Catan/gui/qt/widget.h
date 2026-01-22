#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include "BoardToolbar.h"

class Board;
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:

    Widget(QWidget *parent = nullptr, Board* board = nullptr);
    ~Widget();
protected:
    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
