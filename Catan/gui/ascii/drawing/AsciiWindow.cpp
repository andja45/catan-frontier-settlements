//
// Created by matija on 1/17/26.
//

#include "AsciiWindow.hpp"
#include "AsciiDrawable.hpp"
#include "AsciiComposite.hpp"

AsciiWindow::AsciiWindow(AsciiDrawable *drawable) : AsciiWindow(drawable->getSize()) {
    setRootWidget(drawable);
    padToLeft();
    resizeToFit();
}

AsciiWindow::AsciiWindow(ScreenSize size) {
    m_rootContainer=new AsciiComposite(size);
    m_rootContainer->setOffset({0,0});
    setSize(size);

}

void AsciiWindow::setRootWidget(AsciiDrawable *drawable) {
    m_rootContainer->clearChildren();
    m_rootContainer->appendChild(drawable);
}

void AsciiWindow::setRootWidgetCenter(AsciiDrawable *drawable) {
    m_rootContainer->clearChildren();
    m_rootContainer->addChildCenter(drawable);
}

void AsciiWindow::setMargin(ScreenSize margin) {
    m_rootContainer->setMargin(margin);
    setSize(getSize()+margin+margin);
}

void AsciiWindow::resizeToFit() {
    m_rootContainer->fitPositive();
    m_canvas.setSize(m_rootContainer->getSize());
}

void AsciiWindow::padToLeft() {
    m_rootContainer->padNegative();
    resizeToFit();
}

void AsciiWindow::blit(std::ostream &os) {
    m_rootContainer->draw(m_canvas);
    if (m_border)
        m_rootContainer->drawBorder(m_canvas);
    m_canvas.blit(os);
}
