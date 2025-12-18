//
// Created by andja on 11.12.25..
//

#include "../../../headers/Game/GameModel.h"
#include "../../../headers/Move/Move.h"
#include <random>

bool GameModel::applyMove(const Move& move) {
    if (!move.isValid(*this, m_session))
        return false;

    move.apply(*this, m_session);

    //notifyMoveApplied(move);
    return true;
}

// === Move-ovi koriste ===
bool GameModel::hasResources(int playerId, const ResourcePack& cost) const{
    const Player& p = player(playerId);

    for (const auto& [type, amount] : cost) {
        if (!p.hasResource(type, amount))
            return false;
    }
    return true;
}

void GameModel::consumeResources(int playerId, const ResourcePack& cost){
    Player& p = player(playerId);

    for (const auto& [type, amount] : cost) {
        p.takeResource(type, amount);
    }
}

void GameModel::placeRoad(int playerId, int edgeId){
    Edge* edge = m_board.getEdgeById(edgeId);
    Player& p = player(playerId);

    edge->setRoad(playerId);
    p.addRoad(edge);
}

void GameModel::placeSettlement(int playerId, int nodeId){
    Node* node = m_board.getNodeById(nodeId);
    Player& p = player(playerId);

    node->setOwner(&p);
    p.addHouse(node);
}

void GameModel::upgradeCity(int playerId, int nodeId){
    Node* node = m_board.getNodeById(nodeId);

    node->upgradeToCity();
}

int GameModel::tradeRatioFor(int playerId, ResourceType give) const {
    const Player& p = player(playerId);

    if (p.hasTrade(give))
        return 2;

    if (p.has3for1Trade())
        return 3;

    return 4;
}

int GameModel::rollDice() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(1, 6);

    return dist(gen) + dist(gen);
}

void GameModel::distributeResources(int dice) {
    auto tiles = m_board.getTilesWithNumber(dice);

    for (Tile* tile : tiles) {
        if (tile->isRobberOnTile())
            continue;

        ResourceType res = tile->getType();

        for (Node* node : tile->getAdjacentNodes()) {
            if (!node) continue;

            Player* owner = node->getOwner();
            if (!owner) continue;

            if (node->getNodeType() == NodeType::Settlement) {
                owner->addResource(res, 1);
            }
            else if (node->getNodeType() == NodeType::City) {
                owner->addResource(res, 2);
            }
        }
    }
}

bool GameModel::canPlayerBuildSettlement(int m_player_id, int m_node_id) const {
}
