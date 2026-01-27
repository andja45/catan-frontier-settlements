//
// Created by andja on 16.1.26..
//

#ifndef CATAN_DISCARDCARDS_H
#define CATAN_DISCARDCARDS_H

#include "../Move.h"

class DiscardCardsMove final : public Move{
private:
    ResourcePack m_discarded;
public:
    DiscardCardsMove(PlayerId playerId, const ResourcePack& discarded)
        : Move(playerId), m_discarded(discarded) {}

    MoveType type() const override { return MoveType::DiscardCards; }
    bool isValid(const GameSession& session) const override;
    void apply(GameSession& session) const override;
    const ResourcePack& getDiscarded() const { return m_discarded; }
    // TODO in gui unclickable if <= 0
};

#endif //CATAN_DISCARDCARDS_H