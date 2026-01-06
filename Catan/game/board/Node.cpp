#include "Node.h"

bool Node::is3for1Trade() const {
    return hasTrade() && getTradeResource() == ResourceType::None;
}

std::array<Node *, 3> Node::getIncidentNodes() {
}
