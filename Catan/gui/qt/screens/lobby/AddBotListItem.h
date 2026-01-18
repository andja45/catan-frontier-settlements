#ifndef ADDBOTLISTITEM_H
#define ADDBOTLISTITEM_H

#include <QWidget>


#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
class AddBotListItem : public QWidget {
    Q_OBJECT

private:
    QComboBox *m_difficultyBox;
    QPushButton *m_addBotButton;
public:
    explicit AddBotListItem(QWidget *parent = nullptr);

signals:
    void addBotRequested(const QString &difficulty);

};
#endif // ADDBOTLISTITEM_H
