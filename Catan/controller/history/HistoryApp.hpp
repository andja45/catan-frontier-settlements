//
// Created by matija on 2/4/26.
//

#ifndef CATAN_HOSTAPP_HPP
#define CATAN_HOSTAPP_HPP
#include <common/AbstractApplet.hpp>
#include <common/ApplicationRoot.hpp>
#include <screens/history/GameHistory.h>

class HistoryApp : public AbstractApplet{
public:
    explicit HistoryApp(ApplicationRoot *root)
    : AbstractApplet(root) {
        m_root=root;
        m_historyView=new GameHistory();
        m_historyView->connect(m_historyView,&GameHistory::backToMainMenuRequested,[this]() {
            m_root->showMainMenu();
        });
    }

    ~HistoryApp() override{
        m_historyView->deleteLater();
    }

    void show() override {
        m_historyView->show();
    }

    void hide() override {
        m_historyView->hide();
    }

private:
    GameHistory* m_historyView;
};

#endif //CATAN_HOSTAPP_HPP