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
    const QColor kColorLose = QColor(250, 184, 162);
    const QColor kColorWin= QColor(206, 250, 147);
    std::map<ColumnNames, int> m_nameToCol = {
        {ColumnNames::m_playerNames, 1},
        {ColumnNames::m_date, 0}
    };

    QTableWidget *m_historyTable;
    QPushButton *m_refreshButton;
    QPushButton *m_backButton;
    std::vector<GameData> m_historyGames;
    void loadGameHistoryToTable();
    void colorRow(int row, const QColor &color);
signals:
    void backToMainMenuRequested();
private slots:
    void onRowDoubleClicked(int row, int column);

public:
    GameHistory(QWidget *parent = nullptr);
};

#endif // GAMEHISTORY_H
