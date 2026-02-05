//
// Created by matija on 1/24/26.
//

#ifndef CATAN_GAMECONTROLLER_HPP
#define CATAN_GAMECONTROLLER_HPP

#include <QObject>
#include <memory>
#include <renderstate/ChoosePlayerRenderState.h>
#include <screens/game/GameWindow.h>

#include "GameNetworkAdapter.h"
#include "model/GameSession.h"
#include "renderstate/BoardRenderState.h"
#include "renderstate/ToolbarRenderState.h"

class GameController : public QObject {
    Q_OBJECT
private:
    GameSession& m_session;
    GameWindow& m_gameWindow;
    PlayerId m_localPlayerId;

    bool isLocalPlayersTurn(const char* action) const;
    GameNetworkAdapter* m_adapter;

    BoardRenderState m_boardRenderState;
    ToolbarRenderState m_toolbarRenderState;

    std::unique_ptr<Move> m_activeTool;
    void setActiveTool(std::unique_ptr<Move> tool);
    void clearActiveTool();
    void connectElements();
public:
    GameController(GameSession &session, NetworkTransport* transport, GameWindow &gameWindow, QObject *parent);

    // GLOBAL
    bool sendMove(const Move *move);

    void onError(const std::string &error);

    void updateState();
    void updateActiveToolOnPhase();
public slots:
    // TOOLBAR
    // BUILD:
    void onBuildRoadClicked();
    void onBuildSettlementClicked();
    void onBuildCityClicked();

    // DEVCARDS:
    void onBuyDevCardClicked();
    void onUseDevCardClicked(DevCardType cardType);
    void onStealCardPlayerChosen(PlayerId victimId);
    void onMonopolyResourceChosen(ResourceType resource);
    void onYearOfPlentyResourcesChosen(ResourceType resource1, ResourceType resource2);

    // ROBBER:
    void onDiscardCardsSent(const ResourcePack& discarded);

    // TRADE:
    void onPlayerTradeRequestSent(const ResourcePack& give, const ResourcePack& receive);
    void onPlayerTradeResponseSent(TradeId tradeRequestId);
    void onPlayerTradeAcceptSent(TradeId tradeId, PlayerId acceptedPlayerId);

    void onBankTradeSent(ResourceType give, ResourceType receive);

    // TURN:
    void onRollDiceClicked();
    void onEndTurnClicked();

    // BOARD
    void onBoardElementClicked(int elementId); // in gui sends signal and unhecks button

    // NETWORK
    void onMoveReceived(Move* move);

    void updateView();


signals:
    void buildPlaced();
    void gameClosed();

    void updateToolbar(const ToolbarRenderState&);
    void updateBoard(const BoardRenderState&);
    void updateActivePlayer(int id);

    void setChoosePlayer(const std::unordered_set<PlayerId>& rs);
    void setDiscard();

    void gameOver();
    void gameWon();

    void update();

};

#endif //CATAN_GAMECONTROLLER_HPP
