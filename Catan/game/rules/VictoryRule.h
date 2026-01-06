//
// Created by andja on 6.1.26..
//

#ifndef CATAN_VICTORYRULE_H
#define CATAN_VICTORYRULE_H

#include "Rule.h"

class VictoryRule : public Rule {
public:
    void evaluate(GameSession& session) override;
};

#endif //CATAN_VICTORYRULE_H