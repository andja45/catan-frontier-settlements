//
// Created by andja on 11.12.25.
//

#include "GameModel.h"
#include "../move/Move.h"
#include <random>

void GameModel::notifyModelChanged() {
}

GameModel::GameModel(int numPlayers) {
}

bool GameModel::hasResource(int playerId, ResourceType type, int amount) const {
    return m_players.at(playerId).hasResource(type, amount);
}

bool GameModel::hasResources(int playerId, const ResourcePack& pack) const{
    for (const auto [type, amount] : pack) {
        if (!hasResource(playerId, type, amount))
            return false;
    }
    return true;
}

void GameModel::consumeResource(int playerId, ResourceType type, int amount) {
    m_players.at(playerId).removeResource(type, amount);
}

void GameModel::consumeResources(int playerId, const ResourcePack& pack){
    for (const auto [type, amount] : pack) {
        m_players.at(playerId).removeResource(type, amount);
    }
    notifyModelChanged();
}

void GameModel::transferResource(int playerId, ResourceType type, int amount) {
    m_players.at(playerId).addResource(type, amount);
}

void GameModel::transferResources(int from, int to, const ResourcePack& pack) {
    for (const auto [type, amount] : pack) {
        m_players.at(from).removeResource(type, amount);
        m_players.at(to).addResource(type, amount);
    }
}

void GameModel::stealRandomResource(int thiefId, int victimId) {
    if (m_players.at(victimId).getNumOfResourceCards() == 0) return;
    ResourceType stolen = m_players.at(victimId).takeRandomResource();
    m_players.at(thiefId).addResource(stolen, 1);
}

bool GameModel::canPlaceRoad(int playerId, int edgeId) const {
    const Player& p = m_players.at(playerId);
    if (!p.hasRoadLeft())
        return false;

    Edge* edge = m_board.getEdgeById(edgeId);
    if (!edge)
        return false;

    if (edge->isOccupied())
        return false;

    bool connected = false;
    for (Node* n : edge->getNodes()) {
        if (!n) continue;
        if (n->getOwner() == playerId)
            connected = true;
    }

    for (Edge* e : edge->adjacentEdges()) {
        if (!e) continue;
        if (e->getOwner() == playerId)
            connected = true;
    }

    return connected;
}

bool GameModel::canPlaceSettlement(int playerId, int nodeId, bool isInitialPlacement) const {
    const Player& p = m_players.at(playerId);
    if (!p.hasSettlementLeft()) return false;

    Node* node = m_board.getNodeById(nodeId);
    if (!node) return false;
    if (node->getOwner() != -1) return false;

    for (Node* adj : node->getIncidentNodes()) {
        if (!adj) continue;
        if (adj->getOwner() != -1)
            return false;
    }

    // mora biti povezan putem (osim initial placement faze)
    if (isInitialPlacement)
        return true;

    for (Edge* e : node->getIncidentEdges()) {
        if (e && e->getOwner() == playerId)
            return true;
    }

    return false;
}

bool GameModel::canPlaceCity(int playerId, int nodeId) const {
    const Player& p = m_players.at(playerId);
    if (!p.hasCityLeft()) return false;

    Node* node = m_board.getNodeById(nodeId);
    if (!node) return false;

    // mora vec biti settlement tog igraca
    if (node->getOwner() != playerId) return false;
    if (node->getNodeBuildingType() != NodeType::Settlement) return false;

    return true;
}

bool GameModel::canPlaceRobber(int tileId) const {
    Tile* tile = m_board.getTileById(tileId);
    if (!tile) return false;

    // ne sme bas na svaki tile, ima ogranicenja
    ResourceType type = tile->getResourceType();
    if (type == ResourceType::Desert || type == ResourceType::Sea)
        return false;

    if (tile->isRobberOnTile())
        return false;

    return true;
}

bool GameModel::canStealFrom(int thiefId, int victimId) const {
    if (thiefId == victimId) return false;
    return m_players.at(victimId).getNumOfResourceCards() > 0;
}

void GameModel::placeRoad(int playerId, int edgeId){
    Edge* edge = m_board.getEdgeById(edgeId);

    edge->occupy(playerId);
}

void GameModel::placeSettlement(int playerId, int nodeId){
    Node* node = m_board.getNodeById(nodeId);

    node->setOwner(playerId);
    m_players.at(playerId).addSettlement(node);
}

void GameModel::placeCity(int playerId, int nodeId){
    Node* node = m_board.getNodeById(nodeId);
    node->upgradeToCity();
}

void GameModel::placeRobber(int tileId) {

}

int GameModel::tradeRatioFor(int playerId, ResourceType give) const {
    const Player& player = m_players.at(playerId);

    if (player.hasTrade(give)) return 2;
    if (player.has3for1Trade()) return 3;
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

        ResourceType res = tile->getResourceType();

        for (Node* node : tile->getAdjacentNodes()) {
            if (!node) continue;

            int playerId = node->getOwner();
            if (playerId == -1) continue;

            if (node->getNodeBuildingType() == NodeType::Settlement) {
                m_players.at(playerId).addResource(res, 1);
            } else if (node->getNodeBuildingType() == NodeType::City) {
                m_players.at(playerId).addResource(res, 2);
            }
        }
    }
}