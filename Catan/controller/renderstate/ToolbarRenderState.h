//
// Created by andja on 16.1.26..
//

#ifndef CATAN_TOOLBARRENDERSTATE_H
#define CATAN_TOOLBARRENDERSTATE_H

#include <unordered_set>

#include "RenderState.h"
#include "phase/TurnPhase.h"

enum class ToolbarButton {
    BuildRoad,
    BuildSettlement,
    BuildCity,

    BuyDevCard,
    ActivateDevCard,

    RollDice,
    EndTurn
};

enum class GamePopups {
    StealCard,
    DiscardCards
};

class ToolbarRenderState final : public RenderState {
private:
    std::unordered_set<ToolbarButton> m_enabledButtons;
    std::unordered_set<GamePopups> m_enabledPopups;
public:
    void updateFromPhase(TurnPhase phase); // set + update
    bool isEnabled(ToolbarButton button) const;
    void clear(); // called after endmove applied (everything disabled until renderstate updated again)
};


#endif //CATAN_TOOLBARRENDERSTATE_H