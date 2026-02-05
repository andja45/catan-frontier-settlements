//
// Created by matija on 2/3/26.
//

#include "PopupRenderState.hpp"

void PopupRenderState::updateFromPhase(TurnPhase phase) {
    clear();
    switch (phase) {
    case TurnPhase::DiscardCards:
        m_enabledPopups = {
            PopupType::DiscardCards
        };
    case TurnPhase::SetRobber:
    case TurnPhase::StealCard:
        m_enabledPopups = {
            PopupType::StealCard
        };
    default:
        break;
    }
}

void PopupRenderState::clear() {
    m_enabledPopups.clear();
}

bool PopupRenderState::isEnabled(PopupType popup) const {
    return m_enabledPopups.find(popup) != m_enabledPopups.end();
}

