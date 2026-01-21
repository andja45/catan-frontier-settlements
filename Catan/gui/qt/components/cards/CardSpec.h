#ifndef CARDSPEC_H
#define CARDSPEC_H

#include <QString>
#include <optional>
#include <types/ResourceType.h>

enum class CardKind { Resource, Development };
enum class CardFace { FaceUp, FaceDown };

enum class DevType { Knight, VP, RoadBuilding, YearOfPlenty, Monopoly, Unknown };

struct CardSpec {
    CardKind kind = CardKind::Resource;
    CardFace face = CardFace::FaceDown;

    // exactly one of these is meaningful depending on kind/face
    ResourceType resource = ResourceType::None;
    DevType dev = DevType::Unknown;

    int countBadge = -1;           // 0 -> no badge, >0 -> show small number
    bool emphasized = false;      // e.g., selectable highlight
};

#endif // CARDSPEC_H
