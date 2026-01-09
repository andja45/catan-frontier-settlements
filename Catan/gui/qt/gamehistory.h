#ifndef GAMEHISTORY_H
#define GAMEHISTORY_H

#include <QWidget>
#include <QHeaderView>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "GameDetailsView.h"
#include "../../game/gamemodel/GameData.h"
#include <fstream>
#include <json.hpp>

#include <QBrush>
#include <QColor>

#include<iostream>

enum class ColumnNames{
    m_date,
    m_playerNames

};

class GameHistory : public QWidget
{
    Q_OBJECT

private:
    const QColor m_colorLose = QColor(255, 180, 180);
    const QColor m_colorWin= QColor(180, 255, 180);
    const char* m_historyPath = "../../../resources/history.json";
    std::map<ColumnNames, int> m_nameToCol = {
        {ColumnNames::m_playerNames, 1},
        {ColumnNames::m_date, 0}
    };

    QTableWidget *m_historyTable;
    QPushButton *m_refreshButton;
    QPushButton *m_backButton;
    std::vector<GameData> m_historyGames;
    void loadGameHistoryToTable();
    void setupTable();
    void colorRow(int row, const QColor &color);
signals:
    void backToMainMenuRequested();
private slots:
    void onRowDoubleClicked(int row, int column);

public:
    GameHistory(QWidget *parent = nullptr);
    void loadFromFile(const QString &path);
};

#endif // GAMEHISTORY_H
