
#include <QPainter>
#include <QResizeEvent>
#include <QFont>
#include <QPainterPath>
#include <screens/game/game-overlay/GameOverlay.hpp>
#include <QPropertyAnimation>

#include "common/audio/AudioManager.h"

GameOverlay::GameOverlay(QWidget* parent)
    : QWidget(parent)
{
    m_text="Waiting for other players...";
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_StyledBackground);

    hide();

    if (parent)
        parent->installEventFilter(this);
}

void GameOverlay::showOverlay()
{
    show();
    raise();
    update();
    updateGeometry();
    setWindowOpacity(0.0);

    auto* anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(300);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

}

void GameOverlay::showGameOver() {
    AudioManager::instance().playDefeat();

    m_text="Game Over";
    m_textColor=Qt::white;
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    showOverlay();
}

void GameOverlay::showGameWon() {
    AudioManager::instance().playVictory();

    m_text="You won!";
    m_textColor=Qt::white;
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    showOverlay();

}

void GameOverlay::showWaiting() {
    m_text="Waiting for other players...";
    m_textColor=Qt::white;
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    showOverlay();
}

void GameOverlay::showEmpty() {
    m_text="";
    m_textColor=Qt::white;
    showOverlay();
}

void GameOverlay::overlay(GameOverlayType type) {
    switch (type) {
        case GameOverlayType::GameOver: showGameOver(); break;
        case GameOverlayType::GameWon: showGameWon(); break;
        case GameOverlayType::Waiting: showWaiting(); break;
        case GameOverlayType::Empty: showEmpty(); break;
        case GameOverlayType::Hidden: hideOverlay(); break;
    }
}

void GameOverlay::hideOverlay()
{
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    hide();
}

bool GameOverlay::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == parentWidget() && event->type() == QEvent::Resize) {
        auto* re = static_cast<QResizeEvent*>(event);
        setGeometry(QRect(QPoint(0, 0), re->size()));
    }
    return QWidget::eventFilter(obj, event);
}

void GameOverlay::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.fillRect(rect(), QColor(0, 0, 0, 50));

    QFont font = p.font();
    font.setBold(true);
    font.setPointSize(48);
    p.setFont(font);

    QPainterPath path;

    QRect r = rect();
    QFontMetrics fm(font);
    QRect textRect = fm.boundingRect(r, Qt::AlignCenter, m_text);

    path.addText(
        textRect.left(),
        textRect.top() + fm.ascent(),
        font,
        m_text
    );

    QPen outlinePen(Qt::black);
    outlinePen.setWidth(4);
    outlinePen.setJoinStyle(Qt::RoundJoin);

    p.setPen(outlinePen);
    p.setBrush(Qt::NoBrush);
    p.drawPath(path);

    p.setPen(Qt::NoPen);
    p.setBrush(m_textColor);
    p.drawPath(path);

}

void GameOverlay::updateGeometry()
{
    if (!parentWidget())
        return;
    setGeometry(parentWidget()->rect());
}
