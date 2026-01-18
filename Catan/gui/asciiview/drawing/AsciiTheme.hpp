//
// Created by matija on 1/18/26.
//

#ifndef CATAN_ASCIITHEME_HPP
#define CATAN_ASCIITHEME_HPP

#include <vector>
#include <cstdint>

struct BoardTheme {
    char tileChar = ' ';
    char upDownEdgeChar = '-';
    char upLeftDownRightEdgeChar = '/';
    char upRightDownLeftEdgeChar = '\\';

    char woolChar='w';
    char woodChar='T';
    char brickChar='=';
    char oreChar='o';
    char wheatChar='i';
    char seaChar='~';
    char desertChar=',';

    char settlementChar='*';
    char cityChar='@';

    char robberChar='X';

    uint8_t playerColor[5]{};
    uint8_t outlineColor=1;

    // Delete copy constructor and assignment to prevent multiple instances
    BoardTheme(const BoardTheme&) = delete;
    BoardTheme& operator=(const BoardTheme&) = delete;

    // Provide a static method to access the singleton instance
    static BoardTheme& getInstance() {
        static BoardTheme instance; // Guaranteed to be initialized once
        return instance;
    }

private:
    // Private constructor prevents creating instances outside getInstance
    BoardTheme() = default;
};

#endif //CATAN_ASCIITHEME_HPP