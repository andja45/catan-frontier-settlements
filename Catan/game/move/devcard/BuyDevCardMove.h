//
// Created by andja on 16.1.26..
//

#ifndef CATAN_BUYDEVCARD_H
#define CATAN_BUYDEVCARD_H

#include <random>
#include <types/DevCardType.h>

#include "move/Move.h"

class BuyDevCardMove final : public Move {
friend class MoveProtoFactory;
private:
    DevCardType m_card;
    void setDevCard(DevCardType card) {m_card=card;}
public:
    explicit BuyDevCardMove(PlayerId playerId) : Move(playerId) {
        m_card=DevCardType::None;
    }

    void setDevCard(GameSession& session);
    DevCardType getDevCard() const {return m_card;}

    MoveType type() const override { return MoveType::BuyDevCard; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;

    static DevCardType randomDevCard(DevPack pack, std::mt19937& mt) ;

};


#endif //CATAN_BUYDEVCARD_H