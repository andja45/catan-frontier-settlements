//
// Created by matija on 1/17/26.
//

#ifndef CATAN_ASCIIWINDOW_HPP
#define CATAN_ASCIIWINDOW_HPP

#include <drawing/AsciiDrawable.hpp>
#include <drawing/AsciiCanvas.hpp>
#include <drawing/AsciiComposite.hpp>
#include <drawing/AsciiTypes.hpp>



class AsciiWindow {
private:
    AsciiCanvas m_canvas;
    AsciiComposite* m_rootContainer;
    bool m_border=false;
public:
    AsciiWindow(AsciiDrawable* drawable);
    AsciiWindow(ScreenSize size);

    void setRootWidget(AsciiDrawable *drawable);
    void setRootWidgetCenter(AsciiDrawable *drawable);

    ScreenSize getSize() const {return m_canvas.getSize();}
    void setSize(ScreenSize size) {m_canvas.setSize(size); m_rootContainer->setSize(size);}

    void setMargin(ScreenSize margin);
    void setBorder(bool border) {m_border=border;}

    void resizeToFit();
    void padToLeft();
    //void padToCenter();

    void blit(std::ostream &os);
};

#endif //CATAN_ASCIIWINDOW_HPP