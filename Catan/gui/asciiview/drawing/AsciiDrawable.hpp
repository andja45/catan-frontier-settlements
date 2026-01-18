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
public:
    virtual ~AsciiDrawable() = default;

    AsciiDrawable()=default;
    AsciiDrawable(ScreenCoords offset, ScreenSize size) : m_offset(offset), m_size(size){}
    ScreenCoords getOffset() const {return m_offset;}
    ScreenSize getSize() const {return m_size;}
    void resize(ScreenCoords origin, ScreenSize size); // TODO clear and repaint?
    void recompose(Canvas &canvas, ScreenCoords origin, ScreenSize size);
    void clearArea(Canvas &canvas) const;
    void clearArea(Canvas &canvas,Cell clearColor) const;

    void clearShape(Canvas &canvas) const;
    virtual void render(Canvas &canvas) const = 0;

    void printOut(std::ostream &os) const;
    AsciiWindow toWindow() const;

    void setOffset(ScreenCoords offset) {m_offset=offset;}
    void setSize(ScreenSize size) {m_size=size;}

    ScreenSize getOuterSize() const {return {m_size.width+m_offset.x,m_size.height+m_offset.y};} // virtual for composite?
};

#endif //CATAN_ASCIIDRAWABLE_HPP