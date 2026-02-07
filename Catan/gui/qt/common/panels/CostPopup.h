#ifndef COSTPOPUP_H
#define COSTPOPUP_H

#include"types/ToolbarActionType.h"
#include"FloatingPanel.h"

class QCardRow;

class CostPopup : public FloatingPanel {
    Q_OBJECT
public:
    CostPopup( QWidget* parent = nullptr);
    void setAction(ToolbarActionType action){m_action=action;}

    void refresh();
protected:
    void paintEvent(QPaintEvent*) override;
private:
    ToolbarActionType m_action;
    QCardRow* m_row;

};
#endif // COSTPOPUP_H
