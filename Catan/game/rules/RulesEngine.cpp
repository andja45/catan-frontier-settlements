//
// Created by andja on 6.1.26..
//

#include "RulesEngine.h"

#include "Rule.h"
#include "LargestArmyRule.h"
#include "LongestRoadRule.h"
#include "GameOverRule.h"

RulesEngine::RulesEngine() { // good enough for now
    //m_rules.push_back(std::make_unique<LongestRoadRule>());
    m_rules.push_back(std::make_unique<LargestArmyRule>());
    m_rules.push_back(std::make_unique<GameOverRule>());
}

void RulesEngine::addRule(std::unique_ptr<Rule> rule) {
    m_rules.push_back(std::move(rule));
}

void RulesEngine::evaluate(GameSession& session) const {
    for (auto& rule : m_rules) {
        rule->evaluate(session);
    }
}
