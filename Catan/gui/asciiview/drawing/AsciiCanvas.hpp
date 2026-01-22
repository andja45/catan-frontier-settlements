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

    ScreenCoords m_currentPos={0,0};
    ScreenSize m_currentSize={0,0};

    bool m_strict=false;

    static void printCell(Cell c, std::ostream &os,ScreenCoords scr={0,0});
    bool withinScopeBounds(ScreenCoords pos) const;
    bool withinRelativeScopeBounds(ScreenCoords pos) const;
    bool withinCanvasBounds(ScreenCoords pos)const;
    void allocate();
public:
    void setStrict(bool strict) {m_strict=strict;} // strict doesnt allow drawing outside set scope

    void setSize(const ScreenSize &size);
    ScreenSize getSize() const {return m_size;}

    void blit(std::ostream &os=std::cout) const;
    void blitScoped(std::ostream &os=std::cout) const; // blit only scoped part

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

    Cell getCell(ScreenCoords pos) const {
        if (!withinCanvasBounds(pos)) return {'\0',0};
        return m_cells[pos.x][pos.y];
    }

    void seek(ScreenCoords pos){m_currentPos=pos;}
    void seekAppend(ScreenSize size){m_currentPos=m_currentPos+size;}
    ScreenCoords getSeekPos() const {return m_currentPos;}
    ScreenSize getScopeSize() const {return m_currentSize;}
    void setScopeSize(ScreenSize size){m_currentSize=size;}
    void resetSeek();
};

#endif //CATAN_ASCIIWINDOW_HPP