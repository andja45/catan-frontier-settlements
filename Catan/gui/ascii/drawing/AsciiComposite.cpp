//
// Created by matija on 1/17/26.
//

#include <algorithm>
#include <limits>
#include "AsciiCanvas.hpp"
#include <drawing/AsciiComposite.hpp>

void AsciiComposite::resizeArea(ScreenCoords pos, ScreenSize size) {
    m_size.width=std::max(m_size.width,m_margin.width+pos.x+size.width);
    m_size.height=std::max(m_size.height,m_margin.height+pos.y+size.height);
}

ScreenCoords AsciiComposite::getMinChild() const {
    int minX=std::numeric_limits<int>::max();
    int minY=std::numeric_limits<int>::max();
    for (auto child : m_children) {
        auto[candX,candY]=child->getOffset();
        minX=std::min(minX,m_margin.width+candX);
        minY=std::min(minY,m_margin.height+candY);
    }
    return {minX,minY};
}

// child is fixed in place inside parent independently of child append
void AsciiComposite::addChildFixed(AsciiDrawable *child) {
    //addArea(child->getOffset(),child->getSize());
    m_children.push_back(child);
}

// append child to right of previous child in the same line
void AsciiComposite::appendChild(AsciiDrawable *child) {
    child->setOffset(m_appendOffset+child->getOffset());

    //addArea(child->getOffset(),child->getSize());  // we call this explicitly later if we want, parent doesnt do automatic layout managment
    m_children.push_back(child);

    m_appendOffset.x=child->getOffset().x+child->getSize().width;
    //m_appendOffset.y=m_appendOffset.y; // we keep y in same line (only changes when line brakes)
    m_bottomJump=std::max(m_bottomJump,child->getSize().height+child->getOffset().y); // gets saved for jumping to when line brakes

}

// tries to append child to right if fits, if not append to new line
void AsciiComposite::appendChildAutoBreak(AsciiDrawable *child) {
    auto oldSize=m_size;
    resizeArea(child->getOffset(),child->getSize());
    if (oldSize!=m_size) {
        m_size=oldSize;
        appendCarriageReturn();
    }
    appendChild(child);
}

void AsciiComposite::addChildCenter(AsciiDrawable *child) {
    int offsetX=m_size.width/2-m_margin.width-child->getSize().width/2;
    int offsetY=m_size.height/2-m_margin.height-child->getSize().height/2;
    child->setOffset({offsetX,offsetY});
    addChildFixed(child);
}

// break line if not already at beginning, if you want to add vertical space use seperate method
void AsciiComposite::appendCarriageReturn() {
    if (m_appendOffset.x!=0) {
        m_appendOffset.x=0;
        m_appendOffset.y=m_bottomJump;
    }
}

void AsciiComposite::appendSpacerBelow(int height) {
    appendCarriageReturn();
    m_appendOffset.y+=height;
    //addArea(m_appendOffset,{0,height});
}

void AsciiComposite::appendSpacerRight(int width) {
    m_appendOffset.x+=width;
    //addArea(m_appendOffset,{width,0});
}

// resize to fit children in positive part of canvas, we assume drawing area to be from 0,0 to size
// if you want children to fit entirely first pad negative and than resize to engulf them
void AsciiComposite::fitPositive() {
    for (auto child : m_children) {
        resizeArea(child->getOffset(),child->getSize());
    }
}

// pads margin to make sure no children are offset in negative direction
// if margin is larger we keep it
void AsciiComposite::padNegative() {
    auto [minX,minY]=getMinChild();
    ScreenSize marginAppend;
    if (minX<0) marginAppend.width=-minX;
    if (minY<0) marginAppend.height=-minY;
    
    addMargin(marginAppend);
}

void AsciiComposite::padToZero() {
    auto [minX,minY]=getMinChild();
    addMargin({-minX,-minY});
}

AsciiComposite::AsciiComposite() : AsciiDrawable({0,0},{0,0}) {
    m_appendOffset = {0, 0};
    m_children     = {};
}

AsciiComposite::AsciiComposite(ScreenSize size):AsciiComposite() {
    m_size=size;
}

void AsciiComposite::reset() {
    m_size={0,0};
    m_children.clear();
}

void AsciiComposite::clearChildren() {
    m_children.clear();
}

void AsciiComposite::render(Canvas &canvas) const {
    auto [w,h]=m_margin;
    canvas.seekAppend({w,h});
    for (auto child : m_children) {
        child->draw(canvas);
    }
    canvas.seekAppend({-w,-h});
}

