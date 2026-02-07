//
// Created by matija on 1/27/26.
//

#ifndef CATAN_SKIPPLAYERRULE_HPP
#define CATAN_SKIPPLAYERRULE_HPP
#include <rules/Rule.h>

class SkipPlayerRule: public Rule  {
public:
    void evaluate(GameSession& session) override;

};

#endif //CATAN_SKIPPLAYERRULE_HPP


//
// Created by andja on 6.1.26..
//
