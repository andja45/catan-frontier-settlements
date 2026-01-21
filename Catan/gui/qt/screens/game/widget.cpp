#include <QVBoxLayout>

#include "widget.h"

#include "../../../../cmake-build-debug/gui/qt/catan_gui_qt_autogen/include/ui/ui_widget.h"

#include "board/QBoard.h"

#include <player/Bank.h>
#include <player/Player.h>
#include <sidebar/RightOverlay.h>

Widget::Widget(QWidget *parent, Board* b)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    auto* qboard  = new QBoard(this, b);

    std::vector<Player*> players;
    players.push_back(new Player(1, "Lazar"));
    players.push_back(new Player(1, "Jovana"));
    players.push_back(new Player(1, "Matija + Andjela"));
    players.push_back(new Player(1, "Marko"));

    Bank* bank = new Bank();
    auto* overlay = new RightOverlay(players, bank, this);

    qboard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Make overlay a fixed-ish width right sidebar
    overlay->setMinimumWidth(380);
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
