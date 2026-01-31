
#include <common/theme/GameTheme.h>

std::array<QColor,8> GameTheme::playerColors={
    {
        {192, 57, 43},      // Red
        {41, 128, 185},     // Blue
        {181, 159, 134},    // White
        {241, 196, 15},     // Yellow
        {39, 174, 96},      // Green
        {142, 90, 43},      // Brown
        {44, 44, 44},       // Black
        {143, 57, 179}      // Pink
    }
};

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
}
QColor GameTheme::getFlickerColorByResource(ResourceType resource) {
    switch (resource) {
    case ResourceType::Wood:
        return QColor(255, 230, 120) ;  // warm pale yellow
        break;
    case ResourceType::Brick:
        return QColor(120, 200, 255);   // light cyan / ice blue
        break;
    case ResourceType::Ore:
        return QColor(255, 140, 0);     // orange
        break;
    case ResourceType::Wool:
        return QColor(180, 100, 220);   // soft purple
        break;
    case ResourceType::Wheat:
        return QColor(120, 120, 255);  // periwinkle blue
        break;
    case ResourceType::Desert:
        return QColor(170, 90, 200);   // dusty violet
        break;
    case ResourceType::Sea:
        return QColor(255, 200, 100);   // warm amber
        break;
    case ResourceType::None:
    default:
        return QColor(1,1,1);
        break;
    }
}

QColor GameTheme::getDisabledColorByResource(ResourceType resource) {
    int h, s, v;
    getColorByResource(resource).getHsv(&h, &s, &v);

    s = s / 3;     // more gray
    v = v * 2 / 3; // darker

    return QColor::fromHsv(h, s, v);
}

QColor GameTheme::getPlayerColor(int playerId) {
    return GameTheme::playerColors[playerId % GameTheme::playerColors.size()];
}

QColor GameTheme::getGoldenColor() {
    return QColor(209, 143, 0,200);
}

QColor GameTheme::getGoldenColorStrong() {
    return QColor(209, 143, 0,250);
}

QColor GameTheme::getGrayColor() {
    return QColor(94, 84, 62,80);
}

QColor GameTheme::getGrayColorStrong() {
    return QColor(94, 84, 62,120);
}
