//
// Created by matija on 1/5/26.
//

#include "RandomStandardSmallCreator.hpp"

#include <json.hpp>

struct TileDef;
using json = nlohmann::json;

std::array<int,18> RandomStandardSmallCreator::m_standardNumberOrder = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
const std::array<TileCoords,19> RandomStandardSmallCreator::m_standardCoordinates = {{
    // Center
    { 0,   0},

    // Ring 1 (distance = 1)
    { 1,   0},
    { 1,  -1},
    { 0,  -1},
    {-1,   0},
    {-1,   1},
    { 0,   1},

    // Ring 2 (distance = 2)
    { 1,   1},
    { 2,   0},
    { 2,  -1},
    { 2,  -2},
    { 1,  -2},
    { 0,  -2},
    {-1,  -1},
    {-2,   0},
    {-2,   1},
    {-2,   2},
    {-1,   2},
    { 0,   2}
}
};
const std::array<TileCoords,6> RandomStandardSmallCreator::m_directionCoords = {{
    { 0, -1 },  // top left
    { 1, -1 },  // top right
    { 1,  0 },  // right
    { 0,  1 },  // bottom right
    { -1, 1 },  // bottom left
    { -1, 0 }   // left
}};
static const std::vector<TileDef> m_basicMap = {
    { -1, -2, ResourceType::Wood,   11 },
    {  0, -2, ResourceType::Wool,  12 },
    {  1, -2, ResourceType::Brick,   9 },

    { -2, -1, ResourceType::Wheat,   4 },
    { -1, -1, ResourceType::Ore,     6 },
    {  0, -1, ResourceType::Wood,    5 },
    {  1, -1, ResourceType::Wool,  10 },

    { -2,  0, ResourceType::Brick,   8 },
    { -1,  0, ResourceType::Wheat,   3 },
    {  0,  0, ResourceType::Desert,  0 },
    {  1,  0, ResourceType::Wheat,  11 },
    {  2,  0, ResourceType::Ore,     4 },

    { -1,  1, ResourceType::Wool,   9 },
    {  0,  1, ResourceType::Wood,   10 },
    {  1,  1, ResourceType::Ore,     3 },
    {  2,  1, ResourceType::Brick,   5 },

    {  0,  2, ResourceType::Wool,   6 },
    {  1,  2, ResourceType::Wheat,   8 },
    {  2,  2, ResourceType::Wood,    2 }
};

// generates standard catan map with random resource distribution
std::vector<TileDef> RandomStandardSmallCreator::generateRandomBoard(){
    std::vector<TileDef> r;
    std::vector<ResourceType> hexList = {ResourceType::Desert};
    for(int i = 0; i < 4; i++) {
        hexList.push_back(ResourceType::Wood);
        hexList.push_back(ResourceType::Wheat);
        hexList.push_back(ResourceType::Wool);
    }
    for(int i = 0; i < 3; i++) {
        hexList.push_back(ResourceType::Ore);
        hexList.push_back(ResourceType::Brick);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(hexList.begin(), hexList.end(), g);

    std::reverse(m_standardNumberOrder.begin(), m_standardNumberOrder.end());
    auto hexCoordinates = RandomStandardSmallCreator::m_standardCoordinates;
    int j = 0;
    for(int i = 0; i < hexList.size(); i++){
        if(hexList[i] == ResourceType::Desert) r.push_back({hexCoordinates[i].q(),hexCoordinates[i].r(), hexList[i], 7});
        else r.push_back({hexCoordinates[i].q(),hexCoordinates[i].r(), hexList[i], m_standardNumberOrder[j++]});
    }
    return r;
}

void RandomStandardSmallCreator::populatePorts(Board * board) {
    board->addTrade(NodeCoords({0,-2},PointDirection::LeftTop),ResourceType::None);
    board->addTrade(NodeCoords({0,-2},PointDirection::Top),ResourceType::None);

    board->addTrade(NodeCoords({1,-2},PointDirection::Top),ResourceType::Wood);
    board->addTrade(NodeCoords({1,-2},PointDirection::RightTop),ResourceType::Wood);

    board->addTrade(NodeCoords({2,-1},PointDirection::Top),ResourceType::None);
    board->addTrade(NodeCoords({2,-1},PointDirection::RightTop),ResourceType::None);

    board->addTrade(NodeCoords({-1,-1},PointDirection::LeftBottom),ResourceType::None);
    board->addTrade(NodeCoords({-1,-1},PointDirection::LeftTop),ResourceType::None);

    board->addTrade(NodeCoords({2,0},PointDirection::RightTop),ResourceType::Brick);
    board->addTrade(NodeCoords({2,0},PointDirection::RightBottom),ResourceType::Brick);

    board->addTrade(NodeCoords({-2,1},PointDirection::LeftTop),ResourceType::None);
    board->addTrade(NodeCoords({-2,1},PointDirection::LeftBottom),ResourceType::None);

    board->addTrade(NodeCoords({-2,2},PointDirection::LeftBottom),ResourceType::Wool);
    board->addTrade(NodeCoords({-2,2},PointDirection::Bottom),ResourceType::Wool);

    board->addTrade(NodeCoords({-1,2},PointDirection::Bottom),ResourceType::Wheat);
    board->addTrade(NodeCoords({-1,2},PointDirection::RightBottom),ResourceType::Wheat);

    board->addTrade(NodeCoords({1,1},PointDirection::Bottom),ResourceType::Ore);
    board->addTrade(NodeCoords({1,1},PointDirection::RightBottom),ResourceType::Ore);

    return;
}

std::unique_ptr<Board> RandomStandardSmallCreator::getBoard() {
    auto board=std::make_unique<Board>();
    auto tiles = generateRandomBoard();
    board->initializeBoard(tiles);
    return std::move(board);
}
