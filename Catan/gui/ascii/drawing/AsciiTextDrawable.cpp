//
// Created by matija on 1/23/26.
//

#include "AsciiTextDrawable.hpp"

#include "AsciiCanvas.hpp"

AsciiComposite AsciiTextDrawable::createTextCentered(std::string text, ScreenSize) {
    AsciiComposite comp;
    AsciiTextDrawable* txt=new AsciiTextDrawable(text);
    comp.addChildCenter(txt);
    return comp;
}

void AsciiTextDrawable::render(Canvas &canvas) const {
    for (int i=0; i<m_text.size(); i++) {
        canvas.setCell({i,0},{m_text[i],1});
    }
}
