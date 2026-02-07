#ifndef QCARDROW_H
#define QCARDROW_H

#include <common/cards/QCard.h>
#include <QHBoxLayout>

class QCardRow : public QWidget {
    Q_OBJECT
public:
    explicit QCardRow(QWidget* parent=nullptr) : QWidget(parent) {
        m_layout = new QHBoxLayout(this);
        m_layout->setContentsMargins(0,0,0,0);
        m_layout->setSpacing(6);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        setMinimumHeight(45);
        setMaximumHeight(90);
    }

    explicit QCardRow(std::vector<ResourceType> resources, QWidget* parent=nullptr): QWidget(parent){
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
    void setCardSpec(int i, const CardSpec& spec) {
        if (!m_layout) return;
        if (i < 0 || i >= m_layout->count()) return;

        auto* item = m_layout->itemAt(i);
        if (!item) return;

        auto* card = qobject_cast<QCard*>(item->widget());
        if (!card) return;

        card->setSpec(spec);
    }

    void clear() {
        if (!m_layout) return;

        while ( auto* item = m_layout->takeAt(0)) {
            delete item->widget();
            delete item;
        }
    }

private:
    QHBoxLayout* m_layout = nullptr;
    std::vector<CardSpec> specs() const;
};

#endif // QCARDROW_H
