//
// Created by matija on 1/17/26.
//

#ifndef CATAN_ASCIICANVAS_HPP
#define CATAN_ASCIICANVAS_HPP
#include <drawing/AsciiTypes.hpp>
#include <iostream>

class AsciiDrawable;

class AsciiCanvas {
private:
    CellSpan m_cells{{}};
    ScreenSize m_size{0,0};
    Cell m_backgroundCell{' ',0};

    static void printCell(Cell c, std::ostream &os,ScreenCoords scr={0,0});
    void resize();
public:
    void setSize(ScreenSize &size);
    ScreenSize getSize() const {return m_size;}
    void blit(std::ostream &os=std::cout) const;
    void clear();
    AsciiCanvas();
    AsciiCanvas(ScreenSize size, Cell backgroundCell={' ',0});
    void setBackgroundCell(Cell c) {m_backgroundCell=c;}
    Cell getBackgroundCell() const {return m_backgroundCell;}

    void setCell(ScreenCoords pos, Cell c);
    void setCell(ScreenCoords offset,ScreenCoords pos, Cell c);
    void render(const AsciiDrawable& drawable);
    void setCells(const AsciiDrawable &drawable);
    void setCells(ScreenCoords offset, const CellSpan& cells);
    void setCells(ScreenCoords offset, ScreenSize size,Cell cell);
    void setCells(Cell cell);

    Cell getCell(ScreenCoords pos) const {return m_cells[pos.x][pos.y];}
};

#endif //CATAN_ASCIIWINDOW_HPP