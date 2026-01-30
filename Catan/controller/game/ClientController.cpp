//
// Created by matija on 1/24/26.
//

#include "ClientController.hpp"

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

ClientController::ClientController(GameSession& session, GameNetworkAdapter& adapter, GameWindow &gameWindow, QObject* parent)
    : QObject(parent)
      , m_session(session)
      , m_adapter(adapter) {

    // connects...

    /*connect(&m_adapter, &GameNetworkAdapter::remoteMoveReceived,
        this, &ClientController::onMoveReceived);*/

    update();
}

bool ClientController::isLocalPlayersTurn(const char* action) const {
    if (m_session.currentPlayer() == m_localPlayerId)
        return true;

    qDebug() << action << ": not local player's turn";
    return false;
}

void ClientController::update(){ // filling renderstates and notifying view, only if its my turn
    updateActiveToolOnPhase();

    m_toolbarRenderState.updateFromPhase(m_session.phase());

    if (m_activeTool && m_activeTool->providesAllValid()) {
        m_boardRenderState.setHighlighted(m_activeTool->allValid(m_session),
            m_activeTool->type());
    }

    emit onModelChanged(m_boardRenderState, m_toolbarRenderState);
}

void ClientController::setActiveTool(std::unique_ptr<Move> tool){
    m_activeTool = std::move(tool);
}

void ClientController::clearActiveTool(){
    m_activeTool.reset();
}

void ClientController::updateActiveToolOnPhase(){
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
            clearActiveTool();
            break;
    }
}

void ClientController::onBuildRoadClicked(){
    if (!isLocalPlayersTurn("BuildRoad")) return;

    setActiveTool(std::make_unique<BuildRoadMove>(m_localPlayerId, types::InvalidEdgeId));
    update();
}

void ClientController::onBuildSettlementClicked(){
    if (!isLocalPlayersTurn("BuildSettlement")) return;

    setActiveTool(std::make_unique<BuildSettlementMove>(m_localPlayerId, types::InvalidNodeId));
    update();
}

void ClientController::onBuildCityClicked() {
    if (!isLocalPlayersTurn("BuildCity")) return;

    setActiveTool(std::make_unique<BuildCityMove>(m_localPlayerId, types::InvalidNodeId));
    update();
}

void ClientController::onBoardElementClicked(int elementId){
    if (!isLocalPlayersTurn("BoardElementClicked")) return;

    if (!m_activeTool) return;

    auto* boardMove = dynamic_cast<BoardMove*>(m_activeTool.get());
    if (!boardMove) return;

    boardMove->setBoardElementId(elementId);
    sendMove(m_activeTool.get());
    clearActiveTool(); // if we want to enable multiple builds in a row, we can change this later
}

void ClientController::onBuyDevCardClicked() {
    if (!isLocalPlayersTurn("BuyDev")) return;

    auto move = std::make_unique<BuyDevCardMove>(m_localPlayerId);

    sendMove(move.get());
}

void ClientController::onUseDevCardClicked(DevCardType cardType) {
    if (!isLocalPlayersTurn("UseDev")) return;

    auto move = std::make_unique<PlayDevCardMove>(m_localPlayerId, cardType);

    sendMove(move.get());
}

void ClientController::onStealCardPlayerChosen(PlayerId victimId) {
    if (!isLocalPlayersTurn("StealCard")) return;

    auto move = std::make_unique<StealCardMove>(m_localPlayerId, victimId);

    sendMove(move.get());
}

void ClientController::onMonopolyResourceChosen(ResourceType resource) {
    if (!isLocalPlayersTurn("Monopoly")) return;

    auto move = std::make_unique<MonopolyMove>(m_localPlayerId, resource);

    sendMove(move.get());
}

void ClientController::onYearOfPlentyResourcesChosen(ResourceType resource1, ResourceType resource2) {
    if (!isLocalPlayersTurn("YearOfPlenty")) return;

    auto move = std::make_unique<YearOfPlentyMove>(m_localPlayerId, resource1, resource2);

    sendMove(move.get());
}

void ClientController::onDiscardCardsSent(const ResourcePack &discarded) {
    if (!isLocalPlayersTurn("DiscardCards")) return;

    auto move = std::make_unique<DiscardCardsMove>(m_localPlayerId, discarded);

    sendMove(move.get());
}

void ClientController::onPlayerTradeRequestSent(const ResourcePack &give, const ResourcePack &receive) {
    if (!isLocalPlayersTurn("PlayerTradeRequest")) return;

    auto move = std::make_unique<PlayerTradeRequestMove>(m_localPlayerId, give, receive);

    sendMove(move.get());
}

void ClientController::onPlayerTradeResponseSent(TradeId tradeRequestId) {
    if (!isLocalPlayersTurn("PlayerTradeResponse")) return;

    auto move = std::make_unique<PlayerTradeResponseMove>(m_localPlayerId, tradeRequestId);

    sendMove(move.get());
}

void ClientController::onPlayerTradeAcceptSent(TradeId tradeId, PlayerId acceptedPlayerId) {
    if (!isLocalPlayersTurn("PlayerTradeAccept")) return;

    auto move = std::make_unique<PlayerTradeAcceptMove>(m_localPlayerId, tradeId, acceptedPlayerId);

    sendMove(move.get());
}

void ClientController::onBankTradeSent(ResourceType give, ResourceType receive) {
    if (!isLocalPlayersTurn("BankTrade")) return;

    auto move = std::make_unique<BankTradeMove>(m_localPlayerId, give, receive);

    sendMove(move.get());
}

void ClientController::onRollDiceClicked() {
    if (!isLocalPlayersTurn("RollDice")) return;

    auto move = std::make_unique<RollDiceMove>(m_localPlayerId);

    sendMove(move.get());
}

void ClientController::onEndTurnClicked() {
    if (!isLocalPlayersTurn("EndTurn")) return;

    m_boardRenderState.clear();
    m_toolbarRenderState.clear(); // will remain cleared for local player until his next turn

    auto move = std::make_unique<EndTurnMove>(m_localPlayerId);

    sendMove(move.get());
}

void ClientController::onMoveReceived(Move* receivedMove){
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

void ClientController::sendMove(const Move* move) {
    if (!m_session.applyMove(*move)) {
        qDebug() << "Tried to send invalid move!"; // for testing onl<, will be removed later
        return;
    }

    update();
    m_adapter.sendMove(move);
}
