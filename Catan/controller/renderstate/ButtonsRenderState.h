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

class ButtonsRenderState final : public RenderState {
private:
    TurnPhase m_phase; // game always has a phase, even when initialized
    std::unordered_set<ToolbarButton> m_enabledButtons;
public:
    void updateFromPhase(TurnPhase phase); // set + update
    bool isEnabled(ToolbarButton button) const;
};


#endif //CATAN_TOOLBARRENDERSTATE_H