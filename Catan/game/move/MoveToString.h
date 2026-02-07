#pragma once

#include <string>

class GameSession;
class Move;

std::string moveToString(const Move& move, const GameSession* session = nullptr);
