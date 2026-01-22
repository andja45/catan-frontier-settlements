#ifndef COSTPOPUP_H
#define COSTPOPUP_H
#include<QFrame>
#include<QVBoxLayout>
#include<QLabel>
#include"../../game/types/ToolbarActionType.h"
#include<QVBoxLayout>
#include<QLabel>
#include"QCardRow.h"
#include"FloatingPanel.h"
#include"../../game/gamemodel/MoveCosts.h"

class CostPopup : public FloatingPanel {
    Q_OBJECT
public:
    CostPopup( QWidget* parent = nullptr);
    void setAction(ToolbarActionType action){m_action=action;}
    void refresh();

private:
    ToolbarActionType m_action;
    QCardRow* m_row;

};
#endif // COSTPOPUP_H
