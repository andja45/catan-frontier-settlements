#include "StealCardPopup.h"

#include <qevent.h>

#include "../../../../common/player/PlayerDot.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPen>
#include <common/panels/FloatingButton.hpp>
#include <common/theme/GameTheme.h>

StealCardPopup::StealCardPopup( QWidget* parent)
    : FloatingPanel(parent,Qt::Dialog)
{
    setWindowModality(Qt::ApplicationModal);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_StyledBackground, false);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setStyleSheet(R"(
        QPushButton {
            text-align: left;
            padding: 8px 10px;
            border-radius: 8px;
            background: transparent;
        }
        QPushButton:hover {
            background: rgba(0,0,0,28);
        }
        QPushButton:pressed {
            background: rgba(0,0,0,45);
        }
    )");

    m_root = new QVBoxLayout(this);
    m_root->setContentsMargins(10, 10, 10, 10);
    m_root->setSpacing(8);

    auto* title = new QLabel("Choose a player to rob", this);
    title->setStyleSheet("font-weight: 600;");
    m_root->addWidget(title);
}

void StealCardPopup::setCandidates(const QVector<Player*>& cands) {
    m_cands = cands;
    rebuild();
}

void StealCardPopup::rebuild() {

    while (m_root->count() > 1) {
        auto* item = m_root->takeAt(1);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    for (const auto& c : m_cands) {

        auto* btn=new FloatingButton(this);
        auto* playerLabel=DotAndText::fromPlayer(c);

        btn->addWidget(playerLabel);

        connect(btn, &QPushButton::clicked, this, [this, id=c->getPlayerId()]() {
            emit playerChosen(id);
            closePopup();
        });

        m_root->addWidget(btn);
    }
    adjustSize();
}

void StealCardPopup::openAtGlobal(const QPoint& globalPos) {
    // show near point, but you can clamp to screen later
    move(globalPos + QPoint(12, 12));
    show();
    raise();
    activateWindow();
}

void StealCardPopup::closePopup() {
    hide();
}

void StealCardPopup::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const int r = 10;
    QRectF rr = rect();
    rr.adjust(0.5, 0.5, -0.5, -0.5);

    // Fill
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::white);
    p.drawRoundedRect(rr, r, r);

    // Border
    p.setPen(QPen(QColor("#cccccc"), 1.0));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(rr, r, r);
}


void StealCardPopup::closeEvent(QCloseEvent* event)
{
    event->ignore();
}
