

#include <screens/connect/HostJoinView.h>

HostJoinView::HostJoinView(RoleType type,QWidget *parent)
    : QWidget(parent), m_type(type)
{
    m_gameNameEdit = new QLineEdit(this);
    m_playerNameEdit = new QLineEdit(this);

    m_gameNameEdit->setPlaceholderText("Game name");
    m_playerNameEdit->setPlaceholderText("Your name");


    m_actionButton = new QPushButton( this);

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
    mainLayout->addWidget(m_actionButton);

    if (type == RoleType::Host)
        m_actionButton->setText("Host");
    else
        m_actionButton->setText("Join");

    connect(m_actionButton, &QPushButton::clicked, this, [this]() {
        clearError();
        if(m_gameNameEdit->text()=="")
        {
            onRejected("Please input a game name.");
        }
        else if (m_playerNameEdit->text()=="")
        {
            onRejected("Please input your name.");
        }
        else{
            emit requestSent(
                m_gameNameEdit->text(),
                m_playerNameEdit->text()
                );
        }
    });


}
void HostJoinView::onRejected(const std::string &reason = "Couldn't join game.")
{
    m_errorLabel->setText(QString::fromStdString(reason));
    m_errorLabel->setVisible(true);
}

void HostJoinView::onAccepted() {
    close();
}

void HostJoinView::clearError()
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
