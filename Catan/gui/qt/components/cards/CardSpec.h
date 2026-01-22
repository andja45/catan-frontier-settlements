#ifndef CARDSPEC_H
#define CARDSPEC_H

#include <QString>
#include <types/ResourceType.h>
#include <types/DevCardType.h>

enum class CardKind { Resource, Development };
struct CardSpec {
    CardKind kind = CardKind::Resource;

    // exactly one of these is meaningful depending on kind/face
    ResourceType resource = ResourceType::None;
    DevCardType dev = DevCardType::None;

    int countBadge = -1;           // 0 -> no badge, >0 -> show small number
    bool emphasized = false;      // e.g., selectable highlight
};

#endif // CARDSPEC_H
