//
// Created by andja on 6.1.26..
//

#ifndef CATAN_LARGESTARMYRULE_H
#define CATAN_LARGESTARMYRULE_H

#include "Rule.h"

// rule in charge of keeping track, giving points and calculating who has largest army
class LargestArmyRule : public Rule {
public:
    void evaluate(GameSession& session) override;
};

#endif //CATAN_LARGESTARMYRULE_H