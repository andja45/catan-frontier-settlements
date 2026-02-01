#include <QPainter>
#include <common/player/PlayerDot.h>
#include <common/theme/GameTheme.h>

void PlayerDot::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(Qt::NoPen);
    p.setBrush(m_color);
    p.drawEllipse(rect().adjusted(1,1,-1,-1));
}

QSize PlayerDot::sizeHint() const {
    return {30,30};
}

DotAndText::DotAndText(const QColor &col, const QString &name, QWidget *parent)
        : QWidget(parent), m_color(col)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(6);

    m_dot = new PlayerDot(col, this);

    m_label = new QLabel(name, this);
    m_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    layout->addWidget(m_dot);
    layout->addWidget(m_label);
}

DotAndText * DotAndText::fromPlayer(Player *p, QWidget *parent) {
    return new DotAndText(GameTheme::getPlayerColor(p->getPlayerId()), QString::fromStdString(p->getName()), parent);
}

TextInDot::TextInDot(const QColor &col,const QColor& textCol, const QString &name, QWidget *parent): QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);

    QStackedLayout* layout = new QStackedLayout(this);

    m_dot = new PlayerDot(col, this);
    m_label = new QLabel(getInitials(name.toStdString()).data(), this);
    m_label->setStyleSheet(QString("color:%1").arg(textCol.name()));
    m_label->setFont(QFont("Arial", 12, QFont::Bold));
    m_label->setParent(m_dot);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_label->setFixedSize(m_dot->size()); // match dot size
    m_label->move(0, 0);

    layout->addWidget(m_dot);
    layout->setStackingMode(QStackedLayout::StackAll);
}

void TextInDot::setText(const QString &text) {
    m_label->setText(text);
}

TextInDot * TextInDot::fromPlayer(Player *p, QWidget *parent) {
    auto name=QString::fromStdString(p->getName());
    auto color=GameTheme::getPlayerColor(p->getPlayerId());
    return new TextInDot(color,Qt::white, name, parent);
}

std::string TextInDot::getInitials(const std::string &name) {
    std::istringstream iss(name);
    std::string word;
    std::string initials;

    int count = 0;
    while (iss >> word) {
        if (count < 2) {
            initials += toupper(word[0]);
            count++;
        }
    }
    // if one word take two letters
    if (count == 1 && word.length() >= 2) {
        initials = "";
        for (int i = 0; i < 2; ++i) {
            initials += toupper(name[i]);
        }
    }

    return initials;
}
