//
// Created by matija on 2/3/26.
//

#ifndef CATAN_POPUPRENDERSTATE_HPP
#define CATAN_POPUPRENDERSTATE_HPP
#include <unordered_set>
#include <phase/TurnPhase.h>
#include <renderstate/RenderState.h>
#include <types/ToolbarActionType.h>

enum class PopupType {
    StealCard,
    DiscardCards
};

class PopupRenderState final : public RenderState {

private:
    std::unordered_set<PopupType> m_enabledPopups;
public:
    void updateFromPhase(TurnPhase phase); // set + update
    bool isEnabled(PopupType button) const;
    bool isStealEnabled() const { return isEnabled(PopupType::StealCard); }
    bool isDiscardEnabled() const { return isEnabled(PopupType::DiscardCards); }
    void clear();

};


#endif //CATAN_TOOLBARRENDERSTATE_H