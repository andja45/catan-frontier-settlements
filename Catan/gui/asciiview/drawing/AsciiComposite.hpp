//
// Created by matija on 1/17/26.
//

#ifndef CATAN_ASCIICOMPOSITE_HPP
#define CATAN_ASCIICOMPOSITE_HPP
#include <drawing/AsciiDrawable.hpp>

class AsciiComposite : public AsciiDrawable {
protected:
    ScreenCoords m_appendOffset={0,0}; // for appending children in lines, in typing machine fashion
    int m_bottomJump =0; // not all appended children are same height so we remember current max to jump to new line
    std::vector<AsciiDrawable*> m_children;
    ScreenSize m_margin={0,0}; // margin is added to child offset when rendering and calculating size

    void resizeArea(ScreenCoords pos, ScreenSize size); // resizes to encompass rect
    ScreenCoords getMinChild() const;
public:
    void addChildFixed(AsciiDrawable* child);
    void appendChild(AsciiDrawable* child);
    void appendChildAutoBreak(AsciiDrawable* child);

    void addChildCenter(AsciiDrawable* child);

    void appendCarriageReturn();
    void appendSpacerBelow(int height);
    void appendSpacerRight(int width);

    void fitPositive();
    void padNegative();
    void padToZero();

    // center?

    void setMargin(ScreenSize margin){m_margin=margin;}
    ScreenSize getMargin() const {return m_margin;}
    void addMargin(ScreenSize margin){m_margin=m_margin+margin;}

    AsciiComposite();
    AsciiComposite(ScreenSize size);

    void reset();
    void clearChildren();

    void render(Canvas &canvas) const override;

};

#endif //CATAN_ASCIICOMPOSITE_HPP