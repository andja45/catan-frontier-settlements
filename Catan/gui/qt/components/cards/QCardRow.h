#ifndef QCARDROW_H
#define QCARDROW_H

#include <QCard.h>
#include <QHBoxLayout>

class QCardRow : public QWidget {
    Q_OBJECT
public:
    explicit QCardRow(QWidget* parent=nullptr) : QWidget(parent) {
        m_layout = new QHBoxLayout(this);
        m_layout->setContentsMargins(0,0,0,0);
        m_layout->setSpacing(6);
    }

    QCard* addCard(const CardSpec& spec) {
        auto* c = new QCard(this);
        c->setSpec(spec);
        m_layout->addWidget(c);
        return c;
    }

    void clear() {
        while (auto* item = m_layout->takeAt(0)) {
            delete item->widget();
            delete item;
        }
    }

private:
    QHBoxLayout* m_layout = nullptr;
};

#endif // QCARDROW_H
