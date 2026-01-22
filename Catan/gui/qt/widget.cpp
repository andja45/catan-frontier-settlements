#include <QHBoxLayout>
#include <widget.h>
#include "./ui/ui_widget.h"

#include <QBoard.h>
#include <RightOverlay.h>

Widget::Widget(QWidget *parent, Board* b)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    auto* rootLayout = new QHBoxLayout;
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    auto* qboard  = new QBoard(this, b);
    auto* overlay = new RightOverlay(this);
    auto* toolbar = new BoardToolbar(this);



    toolbar->setMinimumHeight(60);
    toolbar->setAttribute(Qt::WA_StyledBackground, true);
    toolbar->setStyleSheet("background: transparent;");
   toolbar->setAutoFillBackground(false);
    toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    qboard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    overlay->setMinimumWidth(380);
    overlay->setMaximumWidth(600);
    overlay->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    auto* leftBox = new QVBoxLayout;  // ⚠️ NO parent here
    leftBox->setContentsMargins(0, 0, 0, 0);
    leftBox->setSpacing(0);

    leftBox->addWidget(qboard, 1);
    leftBox->addWidget(toolbar, 0);

    rootLayout->addLayout(leftBox, 1);
    rootLayout->addWidget(overlay, 0);

    this->setLayout(rootLayout);

}
void Widget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F11) {
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
        event->accept();
        return;
    }
    if (event->key() == Qt::Key_Escape && isFullScreen()) {
        showNormal();
        event->accept();
        return;
    }

    QWidget::keyPressEvent(event);
}
Widget::~Widget()
{
    delete ui;
}
