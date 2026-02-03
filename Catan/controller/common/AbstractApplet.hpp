//
// Created by matija on 1/27/26.
//

#ifndef CATAN_ABSTRACTAPPLET_HPP
#define CATAN_ABSTRACTAPPLET_HPP


class ApplicationRoot;

class AbstractApplet {
public:
    virtual ~AbstractApplet() = default;

    AbstractApplet(ApplicationRoot* root): m_root(root){}

    virtual void show()=0;;
    virtual void hide()=0;

protected:
    ApplicationRoot* m_root;
};
#endif //CATAN_ABSTRACTAPPLET_HPP