//
// Created by andja on 16.1.26.
//

#include "ToolbarRenderState.h"

void ToolbarRenderState::updateFromPhase(TurnPhase phase) {
    m_phase = phase;
    m_enabledButtons.clear();

    switch (phase) {
        case TurnPhase::RollDice:
            m_enabledButtons.insert(ToolbarButton::RollDice);
            break;

        case TurnPhase::Main:
            m_enabledButtons = {
            ToolbarButton::BuildRoad,
            ToolbarButton::BuildSettlement,
            ToolbarButton::BuildCity,
            ToolbarButton::BuyDevCard,
            ToolbarButton::ActivateDevCard,
            ToolbarButton::EndTurn
        };
            break;

        case TurnPhase::InitialPlacement:
            m_enabledButtons = {
            ToolbarButton::BuildRoad,
            ToolbarButton::BuildSettlement
        };
            break;

        case TurnPhase::RoadBuilding:
            m_enabledButtons.insert(ToolbarButton::BuildRoad); // TODO maybe we want player to have that button automatically selected, to be decided
            break;

        case TurnPhase::YearOfPlenty:
        case TurnPhase::Monopoly:
        case TurnPhase::DiscardCards:
        case TurnPhase::SetRobber:
        case TurnPhase::StealCard:
            break;
    }
}

bool ToolbarRenderState::isEnabled(ToolbarButton button) const {
    return m_enabledButtons.find(button) != m_enabledButtons.end();
}

// maybe setenabled and setselected and then in roadbuilding auto selected
// is buildroad and signal clicked is sent, same as initialplacement, think
// ab it