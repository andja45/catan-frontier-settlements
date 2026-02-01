//
// Created by matija on 1/24/26.
//

#ifndef CATAN_GAMECONTROLLER_HPP
#define CATAN_GAMECONTROLLER_HPP

#include <QObject>
#include <memory>
#include <screens/game/GameWindow.h>

#include "GameNetworkAdapter.h"
#include "model/GameSession.h"
#include "renderstate/RenderState.h"
#include "move/Move.h"
#include "renderstate/BoardRenderState.h"
#include "renderstate/ToolbarRenderState.h"
#
class GameController : public QObject {
    Q_OBJECT
private:
    GameSession& m_session;
    GameNetworkAdapter& m_adapter;

    BoardRenderState m_boardRenderState;
    ToolbarRenderState m_toolbarRenderState;

    std::unique_ptr<Move> m_activeTool;
    void setActiveTool(std::unique_ptr<Move> tool);
    void clearActiveTool();
public:
    GameController(GameSession &session, GameNetworkAdapter &adapter, GameWindow &gameWindow, QObject *parent);

    void sendMove(const Move* move);
public slots:
    // TOOLBAR
    // BUILD:
    void onBuildRoadClicked(PlayerId playerId);
    void onBuildSettlementClicked(PlayerId playerId);
    void onBuildCityClicked(PlayerId playerId);

    // DEVCARDS:

    // TRADE:

    // TURN:


    // BOARD
    void onBoardElementClicked(int elementId);

    // NETWORK
    void onMoveReceived(Move* move);

    // GLOBAL
    void update();

    signals:
        void onModelChanged(const BoardRenderState& state,
                            const ToolbarRenderState& toolbarState);
};


#endif //CATAN_GAMECONTROLLER_HPP