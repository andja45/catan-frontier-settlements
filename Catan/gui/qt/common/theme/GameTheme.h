#ifndef GAMETHEME_H
#define GAMETHEME_H

#include <QColor>
#include <types/ResourceType.h>


class GameTheme
{
public:
    GameTheme();
    static QColor getColorByResource (ResourceType resource);
    static QColor getFlickerColorByResource (ResourceType resource);
    static QColor getDisabledColorByResource (ResourceType resource);
    static QColor getPlayerColor(int playerId);
    static QColor getGoldenColor();
    static QColor getGoldenColorStrong();
    static QColor getGrayColor();
    static QColor getGrayColorStrong();

    static std::array<QColor,8> playerColors;

};

#endif // GAMETHEME_H
