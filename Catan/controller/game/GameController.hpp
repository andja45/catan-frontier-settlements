/*//
// Created by matija on 1/24/26.
//

#ifndef CATAN_GAMECONTROLLER_HPP
#define CATAN_GAMECONTROLLER_HPP

#include <QObject>
#include <memory>

#include "model/GameSession.h"
#include "renderstate/RenderState.h"
#include "network/client/GameNetworkAdapter.h"
#include "move/Move.h"

class ClientController : public QObject {
    Q_OBJECT
private:
    GameSession& m_session;
    RenderState& m_renderState;
    GameNetworkAdapter& m_adapter;

    std::unique_ptr<Move> m_activeTool;
    void setActiveTool(std::unique_ptr<Move> tool);
    void clearActiveTool();
public:
    ClientController(GameSession& session,
                     RenderState& renderState,
                     GameNetworkAdapter& adapter,
                     QObject* parent = nullptr);

public slots:
    // TOOLBAR
    void onBuildRoadClicked();
    void onBuildSettlementClicked();
    void onBuildCityClicked();

    // BOARD
    void onBoardElementClicked(int elementId);

    // NETWORK
    void onMoveReceived(const Move& move);
    void sendMove(const Move& move);

    signals:
        void onRenderStateChanged(const RenderState& state);
};


#endif //CATAN_GAMECONTROLLER_HPP*/