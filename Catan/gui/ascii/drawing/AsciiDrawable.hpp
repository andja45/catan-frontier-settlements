//
// Created by matija on 1/17/26.
//

#ifndef CATAN_ASCIIDRAWABLE_HPP
#define CATAN_ASCIIDRAWABLE_HPP

#include <drawing/AsciiTypes.hpp>
#include <iostream>

class AsciiWindow;


class AsciiDrawable {
protected:
    ScreenCoords m_offset;
    ScreenSize m_size;
    void prepareCanvas(Canvas &canvas) const; // set canvas offset to element offset to prepare for drawing, when called again undo's this
    virtual void render(Canvas &canvas) const = 0;
public:
    virtual ~AsciiDrawable() = default;

    AsciiDrawable()=default;
    AsciiDrawable(ScreenCoords offset, ScreenSize size) : m_offset(offset), m_size(size){}
    ScreenCoords getOffset() const {return m_offset;}
    ScreenSize getSize() const {return m_size;}
    void resize(ScreenCoords origin, ScreenSize size);
    void recompose(Canvas &canvas, ScreenCoords origin, ScreenSize size); // clears area than resizes
    void clearArea(Canvas &canvas) ;
    void clearArea(Canvas &canvas,Cell clearColor);

    void clearShape(Canvas &canvas) const;
    void draw(Canvas &canvas) const; // prepares canvas then renders
    void drawBorder(Canvas &canvas) const;


    void printOut(std::ostream &os);

    AsciiWindow toWindow();

    void setOffset(ScreenCoords offset) {m_offset=offset;}
    void addOffset(ScreenSize size) {m_offset=m_offset+size;}
    void setSize(ScreenSize size) {m_size=size;}

};

#endif //CATAN_ASCIIDRAWABLE_HPP