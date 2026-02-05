//
// Created by matija on 2/3/26.
//

#ifndef CATAN_POPUPSERVICE_HPP
#define CATAN_POPUPSERVICE_HPP

#include <QObject>
#include <QString>
#include <QWidget>

class PopupService {
public:
    explicit PopupService();

    void showError(const QString& title, const QString& message);
    void showWarning(const QString& title, const QString& message);
    void showInfo(const QString& title, const QString& message);

    void setParentWidget(QWidget* parentWidget);
private:
    QWidget* m_parentWidget;
};

#endif //CATAN_POPUPSERVICE_HPP