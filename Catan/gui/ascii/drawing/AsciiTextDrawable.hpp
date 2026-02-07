//
// Created by matija on 1/23/26.
//

#ifndef CATAN_ASCIITEXTDRAWABLE_HPP
#define CATAN_ASCIITEXTDRAWABLE_HPP
#include <drawing/AsciiComposite.hpp>
#include <drawing/AsciiDrawable.hpp>

class AsciiTextDrawable : public AsciiDrawable{
private:
    std::string m_text;
public:
    AsciiTextDrawable(std::string text): m_text(text) {
        m_size={(int)text.length(),1};
    }

    static AsciiComposite createTextCentered(std::string text,ScreenSize);

protected:
    void render(Canvas &canvas) const override;
};

#endif //CATAN_ASCIITEXTDRAWABLE_HPP