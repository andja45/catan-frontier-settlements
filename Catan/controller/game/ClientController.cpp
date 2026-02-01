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
#include "move/robber/SetRobberMove.h"
#include "move/turn/PlayerLeaveMove.hpp"

ClientController::ClientController(GameSession& session, GameNetworkAdapter& adapter, GameWindow &gameWindow, QObject* parent)
    : QObject(parent)
      , m_session(session)
      , m_adapter(adapter) {

    // connects...

    /*connect(&m_adapter, &GameNetworkAdapter::remoteMoveReceived,
        this, &ClientController::onMoveReceived);*/

    update();
}

void ClientController::update(){ // filling renderstates and notifying view, only if its my turn
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

void ClientController::onBuildRoadClicked(PlayerId playerId){
    setActiveTool(std::make_unique<BuildRoadMove>(playerId, types::InvalidEdgeId));
    update();
}

void ClientController::onBuildSettlementClicked(PlayerId playerId){
    setActiveTool(std::make_unique<BuildSettlementMove>(playerId, types::InvalidNodeId));
    update();
}

void ClientController::onBuildCityClicked(PlayerId playerId) {
    setActiveTool(std::make_unique<BuildCityMove>(playerId, types::InvalidNodeId));
    update();
}

void ClientController::onBoardElementClicked(int elementId){
    if (!m_activeTool) {
        return;
    }

    auto* boardMove = dynamic_cast<BoardMove*>(m_activeTool.get());
    if (!boardMove)
        return;

    boardMove->setBoardElementId(elementId);
    sendMove(m_activeTool.get());
    clearActiveTool(); // if we want to enable multiple builds in a row, we can change this later
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
