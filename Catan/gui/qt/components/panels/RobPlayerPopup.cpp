#include "RobPlayerPopup.h"
#include "PlayerDot.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPen>

RobPlayerPopup::RobPlayerPopup(QWidget* parent)
    : FloatingPanel(Qt::Dialog, parent)   // Popup closes when clicking outside
{
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

void RobPlayerPopup::setCandidates(const QVector<Player*>& cands) {
    m_cands = cands;
    rebuild();
}

void RobPlayerPopup::rebuild() {
    while (m_root->count() > 1) {
        auto* item = m_root->takeAt(1);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    for (const auto& c : m_cands) {
        auto* btn = new QPushButton(this);

        // Build a row widget inside the button
        auto* row = new QWidget(btn);
        row->setAttribute(Qt::WA_TransparentForMouseEvents);
        auto* h = new QHBoxLayout(row);
        h->setContentsMargins(8, 0, 8, 0);
        h->setSpacing(8);

        auto* dot = new PlayerDot(row);
        dot->setColor(QColor(255, 140, 60)); // placeholder for now

        auto* name = new QLabel(QString::fromStdString(c->getName()), row);

        h->addWidget(dot);
        h->addWidget(name, 1);

        // Put row widget into button
        auto* outer = new QHBoxLayout(btn);
        outer->setContentsMargins(0, 0, 0, 0);
        outer->addWidget(row);

        connect(btn, &QPushButton::clicked, this, [this, id=c->getPlayerId()]() {
            emit playerChosen(id);
            closePopup();
        });

        m_root->addWidget(btn);
    }

    adjustSize();
}

void RobPlayerPopup::openAtGlobal(const QPoint& globalPos) {
    // show near point, but you can clamp to screen later
    move(globalPos + QPoint(12, 12));
    show();
    raise();
    activateWindow();
}

void RobPlayerPopup::closePopup() {
    hide();
}

void RobPlayerPopup::paintEvent(QPaintEvent*) {
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
