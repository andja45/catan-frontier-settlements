//
// Created by matija on 1/31/26.
//

#ifndef CATAN_GAMEOVERLAY_HPP
#define CATAN_GAMEOVERLAY_HPP

#include <QWidget>
#include <QColor>

class GameOverlay : public QWidget
{
    Q_OBJECT
public:
    explicit GameOverlay(QWidget* parent);


public slots:
    void showOverlay();
    void showGameOver();
    void showGameWon();
    void hideOverlay();


protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QString m_text;
    QColor  m_textColor=Qt::white;
};

#endif //CATAN_GAMEOVERLAY_HPP