#include "FloatingButton.hpp"

#include <iostream>

#include "common/audio/AudioManager.h"

FloatingButton::FloatingButton(QWidget* parent)
    : QPushButton(parent)
{
    setCheckable(false);
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    setStyleSheet(R"(
        FloatingButton {
            background: rgba(245,245,245,230);
            border-radius: 10px;
            color: #2c3e50;
        }

        FloatingButton:hover {
            background: rgba(235,235,235,240);
        }

        FloatingButton:pressed {
            background: rgba(220,220,220,255);
        }

        FloatingButton:checked {
            background: rgba(245,245,245,235);
            color: rgb(80, 140, 200);
            font-weight: 600;
        }

        FloatingButton:disabled {
            background: rgba(200,200,200,200);
            color: rgba(0,0,0,120);
        }
    )");

    m_contentLayout = new QHBoxLayout(this);
    m_contentLayout->setContentsMargins(12, 8, 12, 8);
    m_contentLayout->setSpacing(8);
}

void FloatingButton::addWidget(QWidget* w)
{
    m_contentLayout->addWidget(w,0,Qt::AlignCenter);
}

QSize FloatingButton::sizeHint() const {
    QSize hint = QPushButton::sizeHint();
    if (m_contentLayout) hint = hint.expandedTo(m_contentLayout->sizeHint());
    return hint;
}

QSize FloatingButton::minimumSizeHint() const {
    QSize minHint = QPushButton::minimumSizeHint();
    if (m_contentLayout) minHint = minHint.expandedTo(m_contentLayout->minimumSize());
    return minHint;
}

void FloatingButton::mousePressEvent(QMouseEvent* event) {
    AudioManager::instance().playClick();
    QPushButton::mousePressEvent(event);
}
