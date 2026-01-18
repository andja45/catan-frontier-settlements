//
// Created by matija on 1/17/26.
//

#include <drawing/AsciiWindow.hpp>

AsciiWindow::AsciiWindow(AsciiDrawable *drawable) : AsciiWindow(drawable->getSize()) {
    setRootDrawable(drawable);
}

AsciiWindow::AsciiWindow(ScreenSize size) {
    m_canvas.setSize(size);
    m_rootDrawable=nullptr;
}

void AsciiWindow::setRootDrawable(const AsciiDrawable *drawable) {
    m_rootDrawable=drawable;
}

void AsciiWindow::blit(std::ostream &os) const {
    m_canvas.blit(os);
}
