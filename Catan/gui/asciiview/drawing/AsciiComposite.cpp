//
// Created by matija on 1/17/26.
//

#include <algorithm>
#include <drawing/AsciiComposite.hpp>

void AsciiComposite::addArea(ScreenCoords pos, ScreenSize size) {
    m_offset.x=std::min(m_offset.x,pos.x);
    m_offset.y=std::min(m_offset.y,pos.y);
    m_size.width=std::max(m_offset.x+m_size.width,pos.x+size.width)-m_offset.x;
    m_size.height=std::max(m_offset.y+m_size.height,pos.y+size.height)-m_offset.y;
}

// area updates to fit child, child is fixed in place inside parent independently of child append
void AsciiComposite::addChildFixed(AsciiDrawable *child) {
    addArea(child->getOffset(),child->getSize());
    m_children.push_back(child);
}

// append child to right of previous child in the same line
void AsciiComposite::appendChild(AsciiDrawable *child) {
    child->setOffset(m_appendOffset+child->getOffset());

    addArea(child->getOffset(),child->getSize());
    m_children.push_back(child);

    m_appendOffset.x=child->getOuterSize().width;
    m_appendOffset.y=m_appendOffset.y; // we keep y in same line (only changes when line brakes)
    m_bottomJump=std::max(m_bottomJump,child->getOuterSize().height); // gets saved for jumping to when line brakes

}

void AsciiComposite::appendChildAutoBreak(AsciiDrawable *child) {
    if (child->getOuterSize().width + m_appendOffset.x > m_size.width) {
        appendCarriageReturn();
    }
    appendChild(child);
}

void AsciiComposite::appendCarriageReturn() {
    if (m_appendOffset.x!=0) {
        m_appendOffset.x=0;
        m_appendOffset.y=m_bottomJump;
    }
}

void AsciiComposite::appendSpacerBelow(int height) {
    appendCarriageReturn();
    m_appendOffset.y+=height;
}

void AsciiComposite::appendSpacerRight(int width) {
    m_appendOffset.x+=width;
}

void AsciiComposite::fitToChildren() {
    for (auto child : m_children) {
        addArea(child->getOffset(),child->getSize());
    }
}

AsciiComposite::AsciiComposite() : AsciiDrawable({0,0},{0,0}) {
    m_appendOffset = {0, 0};
    m_children     = {};
}

AsciiComposite::AsciiComposite(ScreenSize size):AsciiComposite() {
    m_size=size;
}

void AsciiComposite::render(Canvas &canvas) const {
    for (auto child : m_children) {
        child->render(canvas);
    }
}

