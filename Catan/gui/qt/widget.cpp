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

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    auto* qboard  = new QBoard(this, b);
    auto* overlay = new RightOverlay(this);

    qboard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Make overlay a fixed-ish width right sidebar
    overlay->setMinimumWidth(400);
    overlay->setMaximumWidth(600);
    overlay->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    layout->addWidget(qboard, 1);   // stretch
    layout->addWidget(overlay, 0);  // fixed width
    setLayout(layout);

    connect(overlay, &RightOverlay::chatSendRequested, this, [overlay](const QString& text){
        overlay->addChatMessage("Me", text);
    });
}

Widget::~Widget()
{
    delete ui;
}
