#ifndef GAMEHOSTVIEW_H
#define GAMEHOSTVIEW_H

#include <QLineEdit>
#include <QSpinBox>
#include <QGroupBox>
#include "PlayerListItem.h"
#include <QRadioButton>
#include "../../game/types/MapType.h"
#include <QListWidgetItem>
#include <QFileDialog>
#include <host-join/RoleType.hpp>
#include <model/GameConfig.h>


class LobbyView : public QWidget {
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
    void disableAll();

    explicit LobbyView(const std::string &gameName,RoleType type, QWidget *parent = nullptr);

    void setPlayerCount(int count);
    void addPlayer(const QString &playerName);
    void removePlayer(const QString &playerName);

    GameConfig getConfig(); //get config from fields
    void setConfig(const GameConfig &config); //set fields from config

signals:
    void startGameRequested(GameConfig config, std::string boardPath="");

    void kickPlayerRequested(const QString &playerName);
    void addBotRequested(const QString &difficulty);
    void customMapSelected(const QString& filePath);

    void configChanged(const GameConfig& config);

public slots:
    void onAddPlayer(const QString &playerName);
    void onRemovePlayer(const QString &playerName);
    void onConfigChanged(const GameConfig &config);
};
#endif // GAMEHOSTVIEW_H
