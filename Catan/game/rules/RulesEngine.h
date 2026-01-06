//
// Created by andja on 6.1.26..
//

#ifndef CATAN_RULESENGINE_H
#define CATAN_RULESENGINE_H

#include "Rule.h"
#include <memory>
#include <vector>

class GameSession;

class RulesEngine {
private:
    std::vector<std::unique_ptr<Rule>> m_rules; // rules engine owns list of rules
public:
    RulesEngine();

    void addRule(std::unique_ptr<Rule> rule); // maybe in future our client-host sends us rules of game he hosts and we add them
    void evaluate(GameSession& session) const;
};

#endif //CATAN_RULESENGINE_H