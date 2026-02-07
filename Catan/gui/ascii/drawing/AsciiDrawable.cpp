//
// Created by matija on 1/17/26.
//

#include "AsciiDrawable.hpp"

#include "AsciiTheme.hpp"

#include "AsciiCanvas.hpp"
#include "AsciiWindow.hpp"

void AsciiDrawable::prepareCanvas(Canvas &canvas) const {
    static bool prepared=false;
    static ScreenSize oldSize={};
    if (!prepared) {
        canvas.seekAppend((Size)m_offset);
        oldSize=canvas.getSize();
        canvas.setScopeSize(m_size);
        prepared=true;
    }
    else {
        canvas.seekAppend({-m_offset.x,-m_offset.y});
        canvas.setScopeSize(oldSize);
        prepared=false;
    }
}

void AsciiDrawable::resize(ScreenCoords origin, ScreenSize size) {
    m_offset=origin;
    m_size=size;
}

void AsciiDrawable::recompose(Canvas &canvas, ScreenCoords origin, ScreenSize size) {
    clearArea(canvas);
    resize(origin,size);
    draw(canvas);
}

void AsciiDrawable::clearArea(Canvas &canvas) {
    clearArea(canvas,canvas.getBackgroundCell());
}

void AsciiDrawable::clearArea(Canvas &canvas, Cell clearColor) {
    prepareCanvas(canvas);
    canvas.setCells(m_offset,m_size,clearColor);
    prepareCanvas(canvas);
}

void AsciiDrawable::clearShape(Canvas &canvas) const {
    AsciiCanvas tempCanvas=AsciiCanvas(m_size,{' ',0});

    render(tempCanvas);
    prepareCanvas(canvas);
    for(int y=0;y<m_size.height;++y){
        for(int x=0;x<m_size.width;++x){
            Cell c=tempCanvas.getCell({x,y});
            if(c.ch!=' '){
                canvas.setCell({x,y},canvas.getBackgroundCell());
            }
        }
    }
    prepareCanvas(canvas);
}

void AsciiDrawable::draw(Canvas &canvas) const {
    prepareCanvas(canvas);
    render(canvas);
    prepareCanvas(canvas);
}

void AsciiDrawable::drawBorder(Canvas &canvas) const {
    auto &theme=BoardTheme::getInstance();
    prepareCanvas(canvas);

    for (int x=0; x<getSize().width; x++) {
        canvas.setCell({x,0},{theme.borderTop,1});
        canvas.setCell({x,m_size.height-1},{theme.borderTop,1});
    }
    for (int y=1; y<m_size.height-1; y++) {
        canvas.setCell({0,y},{theme.borderSides,1});
        canvas.setCell({m_size.width-1,y},{theme.borderSides,1});
    }

    prepareCanvas(canvas);
}

void AsciiDrawable::printOut(std::ostream &os) {
    toWindow().blit(os);
}

AsciiWindow AsciiDrawable::toWindow() {
    auto wnd=AsciiWindow(this);
    return wnd;
}
