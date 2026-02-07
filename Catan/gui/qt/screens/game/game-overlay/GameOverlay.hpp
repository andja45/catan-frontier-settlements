//
// Created by matija on 1/31/26.
//

#ifndef CATAN_GAMEOVERLAY_HPP
#define CATAN_GAMEOVERLAY_HPP

#include <QWidget>
#include <QColor>

enum class GameOverlayType {
    GameOver,
    GameWon,
    Waiting,
    Empty,
    Hidden
};
class GameOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit GameOverlay(QWidget* parent);
    void updateGeometry();

public slots:
    void showOverlay();
    void hideOverlay();

    void showGameOver();
    void showGameWon();
    void showWaiting();
    void showEmpty();

    void overlay(GameOverlayType type);

protected:
    void paintEvent(QPaintEvent* event) override;


    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QString m_text="";
    QColor  m_textColor=Qt::white;
};

#endif //CATAN_GAMEOVERLAY_HPP