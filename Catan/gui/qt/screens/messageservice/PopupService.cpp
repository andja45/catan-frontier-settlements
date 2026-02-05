//
// Created by matija on 2/3/26.
//

#include "PopupService.hpp"
#include <QMessageBox>

PopupService::PopupService()
{
    m_parentWidget = nullptr;
}

void PopupService::showError(const QString& title, const QString& message)
{
    QMessageBox::critical(m_parentWidget, title, message);
}

void PopupService::showWarning(const QString& title, const QString& message)
{
    QMessageBox::warning(m_parentWidget, title, message);
}

void PopupService::showInfo(const QString& title, const QString& message)
{
    QMessageBox::information(m_parentWidget, title, message);
}

void PopupService::setParentWidget(QWidget *parentWidget) {
    m_parentWidget = parentWidget;
}
