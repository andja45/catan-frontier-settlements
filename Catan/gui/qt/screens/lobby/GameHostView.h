#ifndef GAMEHOSTVIEW_H
#define GAMEHOSTVIEW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QGroupBox>
#include "PlayerListItem.h"
#include <QRadioButton>
#include <QButtonGroup>
#include "../../game/types/MapType.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QFileDialog>
#include "AddBotListItem.h"

class GameHostView : public QWidget {
    Q_OBJECT
private:
    QString m_gameName;
    QLabel *m_playerCountLabel;
    QListWidget *m_playerList;
    QString m_customMapPath;
    QLabel *m_errorLabel;

    QSpinBox *m_maxPlayersSpin;
    QSpinBox *m_pointsToWinSpin;

    QButtonGroup *m_mapButtonGroup;
    QRadioButton *m_classicMapRadio;
    QRadioButton *m_extendedMapRadio;
    QRadioButton *m_customMapRadio;
    QPushButton *m_loadBoardButton;
    QPushButton *m_startButton;
    QGroupBox* setMapSelect();
    void addAddBotEntry();
public:
    explicit GameHostView(const std::string &gameName, QWidget *parent = nullptr);

    void setPlayerCount(int count);
    void addPlayer(const QString &playerName);
    void removePlayer(const QString &playerName);
    void clearPlayers();

signals:
    void startGameRequested(const QString &gameName,
                            int maxPlayers,
                            int pointsToWin,
                            const MapType &mapType);
    void kickPlayerRequested(const QString &playerName);
    void addBotRequested(const QString &difficulty);
    void customMapSelected(const QString &jsonPath);


};
#endif // GAMEHOSTVIEW_H
