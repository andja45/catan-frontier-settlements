#ifndef BOARDTOOLBAR_H
#define BOARDTOOLBAR_H

#include<QWidget>
#include<QHBoxLayout>
#include "FloatingPanel.h"
#include<QPushButton>
#include<QButtonGroup>
#include <QMap>
#include<QMenu>
#include <QToolButton>
#include<QPainter>
#include"GameTheme.h"
#include"../../game/types/ToolbarActionType.h"
#include"../../game/types/ResourceType.h"
class BoardToolbar : public QWidget
{Q_OBJECT
private:
    FloatingPanel* m_buttonsPanel;
    QMap<ToolbarActionType, QPushButton*> m_buttons;
    FloatingPanel* createPanelWithButton(QWidget* button);
    QPushButton* createActionButton(const QString& text,ToolbarActionType action);
    void paintEvent(QPaintEvent*);

    void addButton(const QString& text, ToolbarActionType action, QHBoxLayout* layout);
    QToolButton* createMenuButton(const QString& text, const QMap<QString, ToolbarActionType>& actions);
public:
    BoardToolbar(QWidget* parent=nullptr);
signals:
    void actionTriggered(ToolbarActionType action);
};
#endif // BOARDTOOLBAR_H
