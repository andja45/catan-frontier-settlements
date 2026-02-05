#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <common/chat/Chat.hpp>
#include <screens/game/action-popups/ActionManager.hpp>
#include <screens/game/board/QBoard.h>
#include <screens/game/game-overlay/GameOverlay.hpp>
#include <screens/game/sidebar/PlayersOverlay.hpp>
#include <screens/game/toolbar/BoardToolbar.h>
#include <screens/game/trade-pupups/TradeOverlay.hpp>

class Board;

class GameWindow : public QWidget
{
    Q_OBJECT
public:

    GameWindow(Board *board, std::vector<Player *> players, PlayerId currentPlayer, Bank *bank, const std::pair<int,int>*dice,
               std::unordered_map<TradeId,Trade> *trades, QWidget *parent);

    ~GameWindow();

    QBoard* getBoard() { return m_qboard; }
    BoardToolbar* getToolbar() { return m_toolbar; }
    PlayersOverlay* getPlayersOverlay() { return m_playersOverlay; }
    TradeOverlay* getTradeOverlay() { return m_tradeOverlay; }
    ActionManager* getActionManager() { return m_actionManager; }
    Chat* getChat() { return m_chat; }
    GameOverlay* getOverlay() { return m_gameOverlay; }
    void paintEvent(QPaintEvent*);
protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent* event) override;
private:
    QBoard* m_qboard;
    PlayersOverlay* m_playersOverlay;
    BoardToolbar* m_toolbar;
    Chat* m_chat;
    TradeOverlay* m_tradeOverlay;
    ActionManager* m_actionManager;
    GameOverlay* m_gameOverlay;

    Board* m_board;
    std::vector<Player*> m_players;
    PlayerId m_currentPlayerId;
    Player* m_currentPlayer;
    Bank* m_bank;

    std::unordered_map<TradeId,Trade>* m_trades;

signals:
    void closed();
};
#endif // WIDGET_H
