//
// Created by matija on 2/5/26.
//

#include "FloatingTextPill.hpp"


#include <QHBoxLayout>
#include <QEvent>
#include <QResizeEvent>

FloatingTextPill::FloatingTextPill(QWidget* parent)
    : FloatingPanel(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setFocusPolicy(Qt::NoFocus);

    // Label
    m_label = new QLabel(this);
    m_label->setStyleSheet(R"(
        QLabel {
            color: black;
            background: transparent;
        }
    )");
    m_label->setAlignment(Qt::AlignCenter);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 6, 12, 6);
    layout->addWidget(m_label);

    setLayout(layout);
    adjustSize();

    if (parent) {
        parent->installEventFilter(this);
        reposition();
    }
}

void FloatingTextPill::setText(const QString& text)
{
    m_label->setText(text);
    adjustSize();
    reposition();
}

bool FloatingTextPill::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == parentWidget() && event->type() == QEvent::Resize) {
        reposition();
    }
    return FloatingPanel::eventFilter(obj, event);
}

void FloatingTextPill::reposition() // floats at the bottom right
{
    if (!parentWidget())
        return;

    const int x = parentWidget()->width()  - width()  - m_margin;
    const int y = parentWidget()->height() - height() - m_margin;

    move(x, y);
}