//
// Created by andja on 6.1.26..
//

#ifndef CATAN_RULE_H
#define CATAN_RULE_H

class GameSession;

class Rule {
public:
    virtual ~Rule() = default;

    virtual void evaluate(GameSession& session) = 0;
};

#endif //CATAN_RULE_H