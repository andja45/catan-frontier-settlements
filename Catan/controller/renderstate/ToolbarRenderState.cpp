//
// Created by andja on 16.1.26.
//

#include "ToolbarRenderState.h"

void ToolbarRenderState::updateFromPhase(TurnPhase phase) {
    m_enabledButtons.clear();

    switch (phase) {
        case TurnPhase::RollDice:
            m_enabledButtons.insert(ToolbarActionType::RollDice);
            break;

        case TurnPhase::Main:
            m_enabledButtons = {
            ToolbarActionType::BuildRoad,
            ToolbarActionType::BuildSettlement,
            ToolbarActionType::BuildCity,
            ToolbarActionType::BuyDevCard,
            ToolbarActionType::BuyDevCard,
            ToolbarActionType::PlayerTrade,
            ToolbarActionType::BankTrade,
            ToolbarActionType::EndTurn
        };
            break;

        case TurnPhase::InitialPlacement:
            /*m_enabledButtons = { TODO this is if we want our player to click the buttons himself
            ToolbarButton::BuildRoad,
            ToolbarButton::BuildSettlement
        };*/
            break;

        case TurnPhase::RoadBuilding:
            /*m_enabledButtons.insert(ToolbarButton::BuildRoad); // TODO maybe we want player to have that button automatically selected, to be decided
            break;*/

        case TurnPhase::YearOfPlenty:
        case TurnPhase::Monopoly:
        case TurnPhase::DiscardCards:
            m_enabledPopups = {
            GamePopups::DiscardCards
        };
        case TurnPhase::SetRobber:
        case TurnPhase::StealCard:
            m_enabledPopups = {
            GamePopups::StealCard
        };
        case TurnPhase::GameOver:
            break;
    }
}

bool ToolbarRenderState::isEnabled(ToolbarActionType button) const {
    return m_enabledButtons.find(button) != m_enabledButtons.end();
}

void ToolbarRenderState::clear() {
    m_enabledButtons.clear();
    m_enabledPopups.clear();
}

// maybe setenabled and setselected and then in roadbuilding auto selected
// is buildroad and signal clicked is sent, same as initialplacement, think
// ab it