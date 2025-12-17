//
// Created by andja on 10.12.25..
//

#include "../../../headers/Board/Board.h"
#include <memory>
#include <random>
#include <algorithm>


// posto sto jedna ista ivica i node pripadaju vise tajlova moramo ih jednoznacno dodeliti nekom odredjenom po dogovoru
// vise o radialnim koordinatama na: https://www.redblobgames.com/grids/hexagons/
void Board::standardizeCoords(HexCoords &coords, int &index) {
        if (index <= 2) return; //0 1 2 ostaju kako jesu
        //3,4,5 prebaci na odgovarajuci susedni hex i index 0,1,2

        int dq=0, dr=0;
        switch (index) {
            case 3: {
                dq=-1;
                dr=0;
                index=0;
            }
            case 4: {
                dq=-1;
                dr=+1;
                index=1;
            }
            case 5: {
                dq=0;
                dr=+1;
                index=2;
            }
            default: {
                 dq=0;
                dr=0;
            }
        }
        coords.first+=dq;
        coords.second+=dr;
    }

struct TileDef { int q, r; ResourceType res; int number; };

static const std::vector<TileDef> basicMap = {
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


void Board::initializeStandardBoard(std::vector<TileDef> tileMap) {

    m_tiles.clear();
    m_nodes.clear();
    m_edges.clear();
    m_tilesByCoord.clear();
    m_tilesByNumber.clear();

    for (const auto&[q, r, res, number] : basicMap) {
        auto t = std::make_unique<Tile>(q, r, res, number);

        Tile* raw = t.get();
        m_tiles.push_back(std::move(t));
        m_tilesByCoord[{q,r}]=raw;

        if (number >= 2 && number <= 12)
            m_tilesByNumber[number].push_back(raw);
    }

    for ( auto&[coord, uptr] : m_tilesByCoord) {
        Tile* t = uptr;

        for (int corner = 0; corner < 6; corner++) {
            int _corner = corner;
            HexCoords _coord=coord;
            Board::standardizeCoords(_coord,_corner);
            if (m_tilesByCoord[_coord]->getNodeAt(_corner)==nullptr) {
                auto n = std::make_unique<Node>(_coord.first, _coord.second, _corner);
                m_nodes.push_back(std::move(n));
            }
            Node *raw = m_tilesByCoord[_coord]->getNodeAt(_corner);
            t->addAdjacentNode(raw,_corner);
            raw->addAdjacentTile(t,corner);

            static const std::array<std::pair<int,int>, 6> HEX_DIRECTIONS = {{
                {+1,  0},
                {+1, -1},
                { 0, -1},
                {-1,  0},
                {-1, +1},
                { 0, +1}
            }};

            Tile* neighborTile = m_tilesByCoord[{coord.first+HEX_DIRECTIONS[corner].first,coord.second+HEX_DIRECTIONS[corner].second}].get();
            t->addAdjacentTile(neighborTile,corner);
        }
    }
    for ( auto&[coord, uptr] : m_tilesByCoord) {
        for (int corner = 0; corner < 6; corner++) {
            int cornerStart=corner;
            int cornerEnd=(corner+1)%6;
            Node* nStart=getNodeAt(coord,cornerStart);
            Node* nEnd=getNodeAt(coord,cornerStart);

            int _corner=corner;
            HexCoords _coord=coord;
            Board::standardizeCoords(_coord,_corner);
            if (m_tilesByCoord[_coord]->getEdgeAt(_corner)==nullptr) {
                auto e = std::make_unique<Edge>(_coord.first, _coord.second, _corner, nStart, nEnd);
                m_edges.push_back(std::move(e));
                m_tilesByCoord[_coord]->addAdjacentEdge(e.get(),_corner);
            }
            Edge * raw = m_tilesByCoord[_coord]->getEdgeAt(_corner);
            nStart->addAdjacentEdge(raw,0);
            nEnd->addAdjacentEdge(raw,1);
        }
    }
}


void Board::randomBoard(){
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

    auto hexCoordinates = generateCoordinates();
///////////
    int j = 0;
    for(int i = 0; i < hexList.size(); i++){
        if(hexList[i] == ResourceType::Desert) m_tiles.push_back(new Tile(hexList[i], 7, hexCoordinates[i]));
        else m_tiles.push_back(new Tile(hexList[i], m_standardNumberOrder[j++], hexCoordinates[i]));
    }
    for(auto tile : m_tiles) {
        m_tilesByCoord[tile->getTileCoord()] = tile;
    }
}

std::vector<HexCoords> Board::generateCoordinates(){
    std::vector<std::pair<int,int>> hexCoordinates = {
        // Center
        { 0,    0},

        // Ring 1 (distance = 1)
        { 1,   0},
        { 1,  -1},
        { 0,  -1},
        {-1,   0},
        {-1,    1},
        { 0,   1},

        // Ring 2 (distance = 2)
        { 1,   1},
        { 2,   0},
        { 2,  -1},
        { 2,  -2},
        { 1,   -2},
        { 0,   -2},
        {-1,   -1},
        {-2,    0},
        {-2,    1},
        {-2,    2},
        {-1,   2},
        { 0,   2}
    };
    std::reverse(hexCoordinates.begin(), hexCoordinates.end());
    return hexCoordinates;
}

Board::~Board(){
}

Tile* Board::getAdjacent(Tile* tile, std::pair<int,int> dir){
    auto coord = tile->getTileCoord();
    std::get<0>(coord) += std::get<0>(dir);
    std::get<1>(coord) += std::get<1>(dir);

    if(abs(std::get<0>(coord)) == 3) return new Tile();
    if(abs(std::get<1>(coord)) == 3) return new Tile();

    return m_tilesByCoord[coord];
}

std::pair<int,int> Board::directionToCoord(Direction dir){
    switch (dir) {
    case Direction::UpLeft:
        return std::pair<int,int>(-1,  1);
    case Direction::UpRight:
        return std::pair<int,int>(0,  1);
    case Direction::Right:
        return std::pair<int,int>(1,  0);
    case Direction::DownRight:
        return std::pair<int,int>(1, -1);
    case Direction::DownLeft:
        return std::pair<int,int>(0, -1);
    case Direction::Left:
        return std::pair<int,int>(-1, 0);
    }
}
