#include <QVBoxLayout>

#include "GameWindow.h"
#include "board/QBoard.h"

#include <player/Bank.h>
#include <player/Player.h>
#include <screens/game/toolbar/BoardToolbar.h>

GameWindow::GameWindow(Board *board, std::vector<Player *> players, PlayerId currentPlayer, Bank *bank,const std::pair<int,int>*dice,std::unordered_map<TradeId,Trade>* trades, QWidget *parent)
    : QWidget(parent), m_board(board), m_currentPlayerId(currentPlayer), m_bank(bank), m_players(players), m_trades(trades)
{m_currentPlayer=players[currentPlayer];

    // left board+tools right chat+players
    auto* rootLayout = new QHBoxLayout;
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    m_playersOverlay = new PlayersOverlay(m_players, m_bank, m_currentPlayerId, this);
    m_chat = new Chat(this);

    m_qboard = new QBoard(this, board);
    m_qboard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_toolbar = new BoardToolbar(m_currentPlayer,dice,this);
    m_toolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_toolbar->setMinimumHeight(65);

    // setting right overlay
    QWidget* rightOverlay=new QWidget(this);
    rightOverlay->setAttribute(Qt::WA_TranslucentBackground);
    QVBoxLayout* rightOverlayBox = new QVBoxLayout(rightOverlay);
    m_chat->setMaximumHeight(700);
    rightOverlay->setMinimumWidth(380);
    rightOverlay->setMaximumWidth(500);
    rightOverlay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    rightOverlayBox->addWidget(m_chat);
    rightOverlayBox->addStretch(1);
    rightOverlayBox->addWidget(m_playersOverlay);

    // left part board+tool
    auto* leftBox = new QVBoxLayout;
    leftBox->setContentsMargins(0, 0, 0, 0);
    leftBox->setSpacing(0);

    // center board, floating trades
    QWidget* mainArea=new QWidget(this);
    mainArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainArea->setAttribute(Qt::WA_TranslucentBackground);
    QVBoxLayout* centerBox = new QVBoxLayout(mainArea);
    centerBox->addWidget(m_qboard);

    m_tradeOverlay = new TradeOverlay(m_players,currentPlayer,trades,mainArea);
    m_tradeOverlay->raise();

    leftBox->addWidget(mainArea, 1);
    leftBox->addWidget(m_toolbar, 0);

    rootLayout->addLayout(leftBox, 1);
    rootLayout->addWidget(rightOverlay, 0);

    this->setLayout(rootLayout);

    // action popups are anchored to parent floating as dialogs
    m_actionManager=new ActionManager(m_players,m_currentPlayer->getPlayerId(),m_bank,this,this);
    m_gameOverlay=new GameOverlay(this);

    connect(m_toolbar,&BoardToolbar::devCardChosen,
            m_actionManager,&ActionManager::openActionPopup);
}

void GameWindow::paintEvent(QPaintEvent *paint_event) {
    QWidget::paintEvent(paint_event);

    QPainter p(this);
    p.fillRect(rect(), GameTheme::getColorByResource(ResourceType::Sea)); // sea

}

void GameWindow::closeEvent(QCloseEvent *event) {
    emit closed();
    QWidget::closeEvent(event);
}

void GameWindow::keyPressEvent(QKeyEvent* event)
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


GameWindow::~GameWindow(){}


void GameWindow::changeEvent(QEvent* event)
{
    switch (event->type()) {

    case QEvent::WindowStateChange:
        if (!isMinimized()) {
            m_gameOverlay->updateGeometry();
            m_tradeOverlay->updateOverlayGeometry();
        }
        break;

    case QEvent::ActivationChange:
        if (isActiveWindow()) {
            m_gameOverlay->updateGeometry();
            m_tradeOverlay->updateOverlayGeometry();
        }
        break;

    default:
        break;
    }

    QWidget::changeEvent(event);
}
