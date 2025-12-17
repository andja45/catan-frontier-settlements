//
// Created by andja on 10.12.25..
//

#include "../../../headers/Board/Board.h"

#include <memory>


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

void Board::initializeStandardBoard() {
    std::vector<TileDef> basicMap = {
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

    _tiles.clear();
    _nodes.clear();
    _edges.clear();

    for (auto&[fst, snd] : _tilesByNumber) snd.clear();

    for (const auto&[q, r, res, number] : basicMap) {
        auto t = std::make_unique<Tile>(q, r, res, number);

        Tile* raw = t.get();
        _tiles[{q,r}]=(std::move(t));

        if (number >= 2 && number <= 12)
            _tilesByNumber[number].push_back(raw);
    }

    for ( auto&[coord, uptr] : _tiles) {
        Tile* t = uptr.get();

        for (int corner = 0; corner < 6; corner++) {
            int _corner = corner;
            HexCoords _coord=coord;
            Board::standardizeCoords(_coord,_corner);
            if (_tiles[_coord].get()->getNodeAt(_corner)==nullptr) {
                auto n = std::make_unique<Node>(_coord.first, _coord.second, _corner);
                _nodes.push_back(std::move(n));
            }
            Node *raw = _tiles[_coord].get()->getNodeAt(_corner);
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

            Tile* neighborTile = _tiles[{coord.first+HEX_DIRECTIONS[corner].first,coord.second+HEX_DIRECTIONS[corner].second}].get();
            t->addAdjacentTile(neighborTile,corner);
        }
    }
    for ( auto&[coord, uptr] : _tiles) {
        for (int corner = 0; corner < 6; corner++) {
            int cornerStart=corner;
            int cornerEnd=(corner+1)%6;
            Node* nStart=getNodeAt(coord,cornerStart);
            Node* nEnd=getNodeAt(coord,cornerStart);

            int _corner=corner;
            HexCoords _coord=coord;
            Board::standardizeCoords(_coord,_corner);
            if (_tiles[_coord].get()->getEdgeAt(_corner)==nullptr) {
                auto e = std::make_unique<Edge>(_coord.first, _coord.second, _corner, nStart, nEnd);
                _edges.push_back(std::move(e));
                _tiles[_coord].get()->addAdjacentEdge(e.get(),_corner);
            }
            Edge * raw = _tiles[_coord].get()->getEdgeAt(_corner);
            nStart->addAdjacentEdge(raw,0);
            nEnd->addAdjacentEdge(raw,1);
        }
    }
}

