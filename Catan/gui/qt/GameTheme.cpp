#include "GameTheme.h"

GameTheme::GameTheme() {}
QColor GameTheme::getColorByResource(ResourceType resource) {
    switch (resource) {
    case ResourceType::Wood:
        return QColor(34, 139, 34);      // forest green
        break;
    case ResourceType::Brick:
        return QColor(178, 74, 34);      // firebrick red
        break;
    case ResourceType::Ore:
        return QColor(160, 160, 160);    // dim gray
        break;
    case ResourceType::Wool:
        return QColor(144, 238, 144);    // light green
        break;
    case ResourceType::Wheat:
        return QColor(230, 205, 22);     // goldenrod
        break;
    case ResourceType::Desert:
        return QColor(200, 165, 112);     // sand
        break;
    case ResourceType::Sea:
        return QColor(80, 140, 200);     // steel blue
        break;
    case ResourceType::None:
    default:
        return QColor(1,1,1);
        break;
    }
    /*
    switch (h->getResourceType()) {
    case ResourceType::Wood:
        brush = QBrush(QColor(34, 139, 34));      // forest green
        break;
    case ResourceType::Brick:
        brush = QBrush(QColor(178, 74, 34));      // firebrick red
        break;
    case ResourceType::Ore:
        brush = QBrush(QColor(160, 160, 160));    // dim gray
        break;
    case ResourceType::Wool:
        brush = QBrush(QColor(144, 238, 144));    // light green
        break;
    case ResourceType::Wheat:
        brush = QBrush(QColor(230, 205, 22));     // goldenrod
        break;
    case ResourceType::Desert:
        brush = QBrush(QColor(200, 165, 112));     // sand
        break;
    case ResourceType::Sea:
        brush = QBrush(QColor(80, 140, 200));     // steel blue
        break;
    case ResourceType::None:
    default:
        brush = QBrush(Qt::NoBrush);
        break;
    }*/

}
