//
// Created by matija on 1/17/26.
//

#ifndef CATAN_ASCIIWINDOW_HPP
#define CATAN_ASCIIWINDOW_HPP

#include <drawing/AsciiDrawable.hpp>
#include <drawing/AsciiCanvas.hpp>
#include <drawing/AsciiTypes.hpp>



class AsciiWindow {
private:
    AsciiCanvas m_canvas;
    const AsciiDrawable* m_rootDrawable;
public:
    AsciiWindow(AsciiDrawable* drawable);
    AsciiWindow(ScreenSize size);

    void setRootDrawable(const AsciiDrawable* drawable);
    void blit(std::ostream &os) const;
};

#endif //CATAN_ASCIIWINDOW_HPP