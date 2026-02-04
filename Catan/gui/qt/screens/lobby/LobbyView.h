#ifndef LOBBYVIEW_H
#define LOBBYVIEW_H

#include <QWidget>
#include <QString>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QButtonGroup>
#include <QFrame>

#include <RoleType.hpp>
#include <model/GameConfig.h>

class LobbyView : public QWidget {
    Q_OBJECT
private:
    QString m_gameName;

    // Left
    QLabel* m_playerCountLabel = nullptr;
    QListWidget* m_playerList = nullptr;

    // Right - config UI
    QLabel* m_errorLabel = nullptr;

    QButtonGroup* m_mapButtonGroup = nullptr;
    QPushButton* m_normalBtn = nullptr;
    QPushButton* m_extendedBtn = nullptr;
    QPushButton* m_customBtn = nullptr;

    QPushButton* m_loadBoardButton = nullptr;
    QString m_customMapPath;

    QSlider* m_playersSlider = nullptr;
    QLabel*  m_playersBubble = nullptr;

    QSlider* m_pointsSlider = nullptr;
    QLabel*  m_pointsBubble = nullptr;

    QPushButton* m_startButton = nullptr;

    QWidget* makePlayersCard();
    QWidget* makeSettingsCard();
    void addAddBotEntry();

    void setMapButtonsEnabled(bool on);
    void setControlsEnabled(bool on);
public:
    explicit LobbyView(const std::string& gameName, RoleType type, QWidget* parent = nullptr);

    void disableAll();

    void setPlayerCount(int count);
    void addPlayer(const QString& playerName);

    GameConfig getConfig() const;
    void setConfig(const GameConfig &config);

    signals:
        void startGameRequested(GameConfig config, std::string boardPath="");
        void customMapSelected(const QString& filePath);
        void configChanged(const GameConfig& config);

public slots:
    void onAddPlayer(const QString &playerName);
    void onConfigChanged(const GameConfig &config);

private:
    void buildUi(RoleType type);
    QFrame* makeCard();
    QPushButton* makeMapButton(const QString& text);
    QLabel* makeBubbleLabel(const QString& text);
};

#endif // LOBBYVIEW_H
