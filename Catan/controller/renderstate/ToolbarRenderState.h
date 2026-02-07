//
// Created by andja on 16.1.26..
//

#ifndef CATAN_TOOLBARRENDERSTATE_H
#define CATAN_TOOLBARRENDERSTATE_H

#include <unordered_set>
#include <types/ToolbarActionType.h>

#include "RenderState.h"
#include "phase/TurnPhase.h"


enum class GamePopups { // these are popups that arent triggered by click
    StealCard,
    DiscardCards
};

class ToolbarRenderState final : public RenderState {
private:
    std::unordered_set<ToolbarActionType> m_enabledButtons;
    std::unordered_set<GamePopups> m_enabledPopups;
public:
    void updateFromPhase(TurnPhase phase); // set + update
    bool isEnabled(ToolbarActionType button) const;
    void clear(); // called after endmove applied (everything disabled until renderstate updated again)

    bool isEnabled(GamePopups popup) const;

    void disableDev();
};


#endif //CATAN_TOOLBARRENDERSTATE_H