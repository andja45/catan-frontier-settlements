//
// Created by matija on 1/17/26.
//

#include "drawing/AsciiCanvas.hpp"

#include <drawing/AsciiDrawable.hpp>

// ignoring screen coord and cell color for now, might be used later with ncurses
void AsciiCanvas::printCell(Cell c, std::ostream &os,ScreenCoords scr) {
    os<<c.ch;
}

bool AsciiCanvas::withinScopeBounds(ScreenCoords pos) const {
    return pos.x>=m_currentPos.x && pos.y>=m_currentPos.y && pos.x<m_currentPos.x+m_size.width && pos.y<m_currentPos.y+m_size.height;
}

bool AsciiCanvas::withinRelativeScopeBounds(ScreenCoords pos) const {
    return ( pos.x>=0) && pos.y>=0 && pos.x<m_currentSize.width && pos.y<m_currentSize.height;
}

bool AsciiCanvas::withinCanvasBounds(ScreenCoords pos) const {
    return pos.x>=0 && pos.y>=0 && pos.x<m_size.width && pos.y<m_size.height;
}

void AsciiCanvas::allocate() {
    m_cells.clear();
    m_cells.resize(m_size.width,std::vector<Cell>(m_size.height,m_backgroundCell));
}

void AsciiCanvas::setSize(const ScreenSize &size) {
    m_size=size;
    allocate();
}

void AsciiCanvas::blit(std::ostream &os) const {
    for (int y=0; y<m_size.height; y++) {
        for (int x=0; x<m_size.width; x++) {
            printCell(m_cells[x][y],os);
        }
        os<<'\n';
    }
}

void AsciiCanvas::blitScoped(std::ostream &os) const {
    for (int y=m_currentPos.y; y<m_currentPos.y+m_currentSize.height; y++) {
        for (int x=m_currentPos.x; x<m_currentPos.x+m_currentSize.width; x++) {
            printCell(m_cells[x][y],os);
        }
    }
}

void AsciiCanvas::clear() {
    allocate();
}

AsciiCanvas::AsciiCanvas() {
    m_backgroundCell={' ',0};
}

AsciiCanvas::AsciiCanvas(ScreenSize size, Cell backgroundCell):m_backgroundCell(backgroundCell) {
    m_size=size;
    allocate();
}

void AsciiCanvas::setCell(ScreenCoords pos, Cell c) {
    if (m_strict && !withinScopeBounds(pos)) {
        return;
    }
    if (!withinCanvasBounds({pos.x+m_currentPos.x,pos.y+m_currentPos.y})) {
        return;
    }
    m_cells[pos.x+m_currentPos.x][pos.y+m_currentPos.y]=c;
}

void AsciiCanvas::setCell(ScreenCoords offset, ScreenCoords pos, Cell c) {
    setCell({offset.x+pos.x,offset.y+pos.y},c);
}

void AsciiCanvas::setCells(const AsciiDrawable& drawable) {
    drawable.draw(*this);
}

void AsciiCanvas::render(const AsciiDrawable& drawable) {
    setCells(drawable);
}

void AsciiCanvas::setCells(ScreenCoords offset, const CellSpan &cells) {
    for (int i=0; i<cells.size(); i++) {
        for (int j=0; j<cells[i].size(); j++) {
            setCell(offset,{i,j},cells[i][j]);
        }
    }
}

void AsciiCanvas::setCells(ScreenCoords offset, Size size, Cell cell) {
    auto cellSpan=std::vector(size.width,std::vector<Cell>(size.height,cell));
    setCells(offset,cellSpan);
}

void AsciiCanvas::setCells(Cell cell) {
    setCells({0,0},m_size,cell);
}

void AsciiCanvas::resetSeek() {
    m_currentSize=m_size;
    m_currentPos={0,0};
}

