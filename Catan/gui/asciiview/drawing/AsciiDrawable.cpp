//
// Created by matija on 1/17/26.
//

#include "drawing/AsciiDrawable.hpp"
#include "AsciiCanvas.hpp"
#include "AsciiWindow.hpp"

void AsciiDrawable::resize(ScreenCoords origin, ScreenSize size) {
    m_offset=origin;
    m_size=size;
}

void AsciiDrawable::recompose(Canvas &canvas, ScreenCoords origin, ScreenSize size) {
    clearArea(canvas);
    resize(origin,size);
    render(canvas);
}

void AsciiDrawable::clearArea(Canvas &canvas) const{
    clearArea(canvas,canvas.getBackgroundCell());
}

void AsciiDrawable::clearArea(Canvas &canvas, Cell clearColor) const {
    canvas.setCells(m_offset,m_size,clearColor);

}

void AsciiDrawable::clearShape(Canvas &canvas) const {
    AsciiCanvas tempCanvas=AsciiCanvas(m_size,{' ',0});
    render(tempCanvas);
    for(int y=0;y<m_size.height;++y){
        for(int x=0;x<m_size.width;++x){
            Cell c=tempCanvas.getCell({x,y});
            if(c.ch!=' '){
                canvas.setCell({m_offset.x+x,m_offset.y+y},canvas.getBackgroundCell());
            }
        }
    }
}

void AsciiDrawable::printOut(std::ostream &os) const {
    toWindow().blit(os);
}

AsciiWindow AsciiDrawable::toWindow() const {
    auto window = AsciiWindow(m_size);
    window.setRootDrawable(this);
    return window;
}
