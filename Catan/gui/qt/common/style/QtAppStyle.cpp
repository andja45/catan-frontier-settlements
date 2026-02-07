#include "QtAppStyle.hpp"

#include <QFile>
#include <QPalette>
#include <QStyleFactory>

namespace catan::gui::qt {

void applyDefaultQtStyle(QApplication& app)
{
    // set up uniform look
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, Qt::white);
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::Base, Qt::white);
    lightPalette.setColor(QPalette::AlternateBase, QColor(245, 245, 245));
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Highlight, QColor(0, 120, 215)); // standard blue
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);
    lightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    lightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
    app.setPalette(lightPalette);

    QFile file(":/styles/catanbuttons.qss");
    if (file.open(QFile::ReadOnly)) {
        app.setStyleSheet(file.readAll());
    }
}

}

