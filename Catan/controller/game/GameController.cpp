//
// Created by matija on 1/24/26.
//

#include "GameController.hpp"

#include <utility>
#include <QDebug>

#include "move/BoardMove.h"
#include "move/build/BuildCityMove.h"
#include "move/build/BuildRoadMove.h"
#include "move/build/BuildSettlementMove.h"
#include "move/devcard/BuyDevCardMove.h"
#include "move/devcard/MonopolyMove.h"
#include "move/devcard/YearOfPlentyMove.h"
#include "move/robber/DiscardCardsMove.h"
#include "move/robber/SetRobberMove.h"
#include "move/robber/StealCardMove.h"
#include "move/trade/PlayerTradeAcceptMove.h"
#include "move/trade/PlayerTradeRequestMove.h"
#include "move/trade/PlayerTradeResponseMove.h"
#include "move/turn/EndTurnMove.h"
#include "move/turn/PlayerLeaveMove.hpp"
#include "move/turn/RollDiceMove.h"

GameController::GameController(GameSession& session, NetworkTransport* transport, GameWindow &gameWindow, QObject* parent)
    : QObject(parent), m_session(session)
    {
    m_adapter = new GameNetworkAdapter(this);
    m_adapter->setTransport(transport);

    auto qboard= gameWindow.getBoard();
    auto qtoolbar= gameWindow.getToolbar();
    auto qchat = gameWindow.getChat();
    auto qtradeOverlay = gameWindow.getTradeOverlay();
    auto qactionManager = gameWindow.getActionManager();
    auto qgameOverlay = gameWindow.getOverlay();
    auto qplayersOverlay = gameWindow.getPlayersOverlay();

    connect(m_adapter,&GameNetworkAdapter::remoteMoveReceived,
            this, &GameController::onMoveReceived);
    connect(&gameWindow,&GameWindow::closed,this,[&session, this]() {
        m_adapter->sendMove( new PlayerLeaveMove(session.localPlayer()));
        emit gameClosed();
    });

    connect(m_adapter,&GameNetworkAdapter::remoteMessageReceived,
            qchat, [qchat](std::string a,std::string m) {
                qchat->addChatMessage(QString::fromStdString(a), QString::fromStdString(m));
            });
    connect(qchat,&Chat::chatSendRequested,this, [this](const QString& msg) {
        m_adapter->sendMessage("",msg.toStdString());
    });

    connect(qtoolbar,&BoardToolbar::buildRoadClicked,
            this,&GameController::onBuildRoadClicked);
    connect(qtoolbar,&BoardToolbar::buildSettlementClicked,
            this,&GameController::onBuildSettlementClicked);
    connect(qtoolbar,&BoardToolbar::buildCityClicked,
            this,&GameController::onBuildCityClicked);

    connect(qtoolbar,&BoardToolbar::buyDevCardRequested,
            this,&GameController::onBuyDevCardClicked);
    connect(qtoolbar,&BoardToolbar::endTurnRequested,
            this,&GameController::onEndTurnClicked);
    connect(qtoolbar,&BoardToolbar::diceRolled,
            this,&GameController::onRollDiceClicked);
    connect(qtoolbar,&BoardToolbar::devCardChosen,
            this,&GameController::onUseDevCardClicked);

    connect(qtoolbar,&BoardToolbar::bankTradeRequested,this,[this](TradeOffer offer) {
        onBankTradeSent(offer.give.begin()->first, offer.receive.begin()->first);
    });
    connect(qtoolbar,&BoardToolbar::playerTradeRequested,this,[this](const TradeOffer& offer) {
        onPlayerTradeRequestSent(offer.give,offer.receive);
    });
    connect(qtoolbar,&BoardToolbar::devCardChosen,this,&GameController::onUseDevCardClicked);

    connect(qboard,&QBoard::elementClicked,this,&GameController::onBoardElementClicked);
    connect(this,&GameController::buildPlaced,qboard,&QBoard::onPlayBuildFeedback);

    connect(this,&GameController::gameOver,qgameOverlay,&GameOverlay::showGameOver);
    connect(this,&GameController::gameWon,qgameOverlay,&GameOverlay::showGameWon);

    connect(this,&GameController::updateToolbar,qtoolbar,&BoardToolbar::updateState);
    connect(this,&GameController::updateBoard,qboard,&QBoard::update);
    connect(this,&GameController::updateActivePlayer,qplayersOverlay,&PlayersOverlay::setActivePlayer);
    connect(this,&GameController::updateChoosePlayer,qactionManager,&ActionManager::setStealCandidates);
    connect(this,&GameController::redraw,qtradeOverlay,&TradeOverlay::refresh);
    connect(this,&GameController::setDiscard,qactionManager,&ActionManager::openDiscardPopup);

    update();

}

bool GameController::isLocalPlayersTurn(const char* action) const {
    if (m_session.currentPlayer() == m_localPlayerId)
        return true;

    qDebug() << action << ": not local player's turn";
    return false;
}

void GameController::update(){ // filling renderstates and notifying view, only if its my turn
    updateActiveToolOnPhase();

    m_toolbarRenderState.updateFromPhase(m_session.phase());

    if (m_activeTool && m_activeTool->providesAllValid()) {
        m_boardRenderState.setHighlighted(m_activeTool->allValid(m_session),
            m_activeTool->type());
    }

    emit onModelChanged(m_boardRenderState, m_toolbarRenderState);
}

void GameController::setActiveTool(std::unique_ptr<Move> tool){
    m_activeTool = std::move(tool);
}

void GameController::clearActiveTool(){
    m_activeTool.reset();
}

void GameController::updateActiveToolOnPhase(){
    switch (m_session.phase()) { // these are activetools that become active when force phase change, not clicking
        case TurnPhase::SetRobber:
            setActiveTool(std::make_unique<SetRobberMove>(m_localPlayerId, types::InvalidTileId));
            break;

        case TurnPhase::StealCard:
            setActiveTool(std::make_unique<StealCardMove>(m_localPlayerId, types::InvalidPlayerId));
            break;

        case TurnPhase::RoadBuilding: // remove this if we want player to click on road himself (uncomment required toolbarrenderstate code)
            setActiveTool(std::make_unique<BuildRoadMove>(m_localPlayerId, types::InvalidEdgeId));
            break;

        case TurnPhase::InitialPlacement:
            if (m_session.lastMoveType() == MoveType::InvalidMoveType || // for first move of the game
                m_session.lastMoveType() == MoveType::BuildRoad)
                setActiveTool(std::make_unique<BuildSettlementMove>(m_localPlayerId, types::InvalidNodeId));
            else if (m_session.lastMoveType() == MoveType::BuildSettlement)
                setActiveTool(std::make_unique<BuildRoadMove>(m_localPlayerId, types::InvalidEdgeId));
            break;

        default:
            break;
    }
}

void GameController::onBuildRoadClicked(){
    if (!isLocalPlayersTurn("BuildRoad")) return;

    setActiveTool(std::make_unique<BuildRoadMove>(m_localPlayerId, types::InvalidEdgeId));
    update();
}

void GameController::onBuildSettlementClicked(){
    if (!isLocalPlayersTurn("BuildSettlement")) return;

    setActiveTool(std::make_unique<BuildSettlementMove>(m_localPlayerId, types::InvalidNodeId));
    update();
}

void GameController::onBuildCityClicked() {
    if (!isLocalPlayersTurn("BuildCity")) return;

    setActiveTool(std::make_unique<BuildCityMove>(m_localPlayerId, types::InvalidNodeId));
    update();
}

void GameController::onBoardElementClicked(int elementId){
    if (!isLocalPlayersTurn("BoardElementClicked")) return;

    if (!m_activeTool) return;

    auto* boardMove = dynamic_cast<BoardMove*>(m_activeTool.get());
    if (!boardMove) return;

    boardMove->setBoardElementId(elementId);
    sendMove(m_activeTool.get());

    emit buildPlaced(); // shake only after successful placement
}

void GameController::onBuyDevCardClicked() {
    if (!isLocalPlayersTurn("BuyDev")) return;

    auto move = std::make_unique<BuyDevCardMove>(m_localPlayerId);

    sendMove(move.get());
}

void GameController::onUseDevCardClicked(DevCardType cardType) {
    if (!isLocalPlayersTurn("UseDev")) return;

    auto move = std::make_unique<PlayDevCardMove>(m_localPlayerId, cardType);

    sendMove(move.get());
}

void GameController::onStealCardPlayerChosen(PlayerId victimId) {
    if (!isLocalPlayersTurn("StealCard")) return;

    if (!m_activeTool) return;

    auto* move = dynamic_cast<StealCardMove*>(m_activeTool.get());
    if (!move) return;

    move->setBoardElementId(victimId);
    sendMove(m_activeTool.get());
}

void GameController::onMonopolyResourceChosen(ResourceType resource) {
    if (!isLocalPlayersTurn("Monopoly")) return;

    auto move = std::make_unique<MonopolyMove>(m_localPlayerId, resource);

    sendMove(move.get());
}

void GameController::onYearOfPlentyResourcesChosen(ResourceType resource1, ResourceType resource2) {
    if (!isLocalPlayersTurn("YearOfPlenty")) return;

    auto move = std::make_unique<YearOfPlentyMove>(m_localPlayerId, resource1, resource2);

    sendMove(move.get());
}

void GameController::onDiscardCardsSent(const ResourcePack &discarded) {
    if (!isLocalPlayersTurn("DiscardCards")) return;

    auto move = std::make_unique<DiscardCardsMove>(m_localPlayerId, discarded);

    sendMove(move.get());
}

void GameController::onPlayerTradeRequestSent(const ResourcePack &give, const ResourcePack &receive) {
    if (!isLocalPlayersTurn("PlayerTradeRequest")) return;

    auto move = std::make_unique<PlayerTradeRequestMove>(m_localPlayerId, give, receive);

    sendMove(move.get());
}

void GameController::onPlayerTradeResponseSent(TradeId tradeRequestId) {
    if (!isLocalPlayersTurn("PlayerTradeResponse")) return;

    auto move = std::make_unique<PlayerTradeResponseMove>(m_localPlayerId, tradeRequestId);

    sendMove(move.get());
}

void GameController::onPlayerTradeAcceptSent(TradeId tradeId, PlayerId acceptedPlayerId) {
    if (!isLocalPlayersTurn("PlayerTradeAccept")) return;

    auto move = std::make_unique<PlayerTradeAcceptMove>(m_localPlayerId, tradeId, acceptedPlayerId);

    sendMove(move.get());
}

void GameController::onBankTradeSent(ResourceType give, ResourceType receive) {
    if (!isLocalPlayersTurn("BankTrade")) return;

    auto move = std::make_unique<BankTradeMove>(m_localPlayerId, give, receive);

    sendMove(move.get());
}

void GameController::onRollDiceClicked() {
    if (!isLocalPlayersTurn("RollDice")) return;

    auto move = std::make_unique<RollDiceMove>(m_localPlayerId);

    sendMove(move.get());
}

void GameController::onEndTurnClicked() {
    if (!isLocalPlayersTurn("EndTurn")) return;

    m_boardRenderState.clear();
    m_toolbarRenderState.clear(); // will remain cleared for local player until his next turn

    auto move = std::make_unique<EndTurnMove>(m_localPlayerId);

    sendMove(move.get());
}

void GameController::onMoveReceived(Move* receivedMove){
    std::unique_ptr<Move> move(receivedMove);
    if (!move) return;

    if (!m_session.applyMove(*move)) {
        qDebug() << "Received invalid move from network!";
        auto leaveMove = std::make_unique<PlayerLeaveMove>(m_session.localPlayer());
        sendMove(leaveMove.get());
        return;
    }

    emit onModelChanged(m_boardRenderState, m_toolbarRenderState); // they will be cleared since im not active player rn, this will only make view redraw for me since someone else played
}

void GameController::onModelChanged(const BoardRenderState &state, const ToolbarRenderState &toolbarState) {
    emit updateActivePlayer(m_session.currentPlayer());
    emit updateToolbar(toolbarState);
    emit updateBoard(state);
    emit redraw();

}

void GameController::sendMove(const Move* move) {
    if (!m_session.applyMove(*move)) {
        qDebug() << "Tried to send invalid move!"; // for testing onl<, will be removed later
        return;
    }

    clearActiveTool(); // tool isnt active anymore after sending move (even sequential moves are made of multiple moves)
    update();
    m_adapter->sendMove(move);
}
