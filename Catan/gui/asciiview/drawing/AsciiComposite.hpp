//
// Created by matija on 1/17/26.
//

#ifndef CATAN_ASCIICOMPOSITE_HPP
#define CATAN_ASCIICOMPOSITE_HPP
#include <drawing/AsciiDrawable.hpp>

class AsciiComposite : public AsciiDrawable {
protected:
    ScreenCoords m_appendOffset;
    int m_bottomJump =0;
    std::vector<AsciiDrawable*> m_children;
    void addArea(ScreenCoords pos, ScreenSize size);
public:
    void addChildFixed(AsciiDrawable* child);
    void appendChild(AsciiDrawable* child);
    void appendChildAutoBreak(AsciiDrawable* child);

    void appendCarriageReturn();
    void appendSpacerBelow(int height);
    void appendSpacerRight(int width);

    void fitToChildren();

    AsciiComposite();
    AsciiComposite(ScreenSize size);

    void render(Canvas &canvas) const override;

};

#endif //CATAN_ASCIICOMPOSITE_HPP