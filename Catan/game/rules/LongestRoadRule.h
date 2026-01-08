//
// Created by andja on 6.1.26..
//

#ifndef CATAN_LONGESTROADRULE_H
#define CATAN_LONGESTROADRULE_H


#include "Rule.h"

// rule in charge of keeping track, giving points and calculating who has longest road
class LongestRoadRule : public Rule {
public:
    void evaluate(GameSession& session) override;
};


#endif //CATAN_LONGESTROADRULE_H