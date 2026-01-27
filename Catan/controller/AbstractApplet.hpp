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

    virtual void start() = 0;
    virtual void exit() = 0;

    virtual void switchToNext()=0;
    virtual void switchToPrevious()=0;

protected:
    ApplicationRoot* m_root;
};
#endif //CATAN_ABSTRACTAPPLET_HPP