#include "GameJoinView.h"

GameJoinView::GameJoinView(QWidget *parent)
    : QWidget(parent)
{
    m_gameNameEdit = new QLineEdit(this);
    m_playerNameEdit = new QLineEdit(this);

    m_gameNameEdit->setPlaceholderText("Game name");
    m_playerNameEdit->setPlaceholderText("Your name");

    m_joinButton = new QPushButton("Join Game", this);

    m_errorLabel = new QLabel("That game does not exist.", this);
    m_errorLabel->setStyleSheet("color: red;");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setVisible(false);

    auto *formLayout = new QFormLayout;
    formLayout->addRow("Game:", m_gameNameEdit);
    formLayout->addRow("Player:", m_playerNameEdit);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(m_errorLabel);
    mainLayout->addWidget(m_joinButton);

    connect(m_joinButton, &QPushButton::clicked, this, [this]() {
        clearError();
        if(m_gameNameEdit->text()=="")
        {
            onJoinRejected("Please input a game name.");
        }
        else if (m_playerNameEdit->text()=="")
        {
            onJoinRejected("Please input your name.");
        }
        else{
            emit joinGameRequested(
                m_gameNameEdit->text(),
                m_playerNameEdit->text()
                );
        }
    });
}
void GameJoinView::onJoinRejected(const std::string &reason = "Couldn't join game.")
{
    m_errorLabel->setText(QString::fromStdString(reason));
    m_errorLabel->setVisible(true);
}

void GameJoinView::clearError()
{
    m_errorLabel->setVisible(false);
}

/*
 * #TODO: add za kontroler:
connect(joinView, &GameJoinView::joinGameRequested,
        controller, &GameController::tryJoinGame);

connect(controller, &GameController::joinRejected,
        joinView, &GameJoinView::onJoinRejected);

 * **/
