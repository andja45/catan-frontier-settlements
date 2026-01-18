#ifndef GAMETHEME_H
#define GAMETHEME_H

#include <QColor>
#include "../../../game/types/ResourceType.h"


class GameTheme
{
public:
    GameTheme();
    static QColor getColorByResource (ResourceType resource);
};

#endif // GAMETHEME_H
