#include "PlayerListItem.h"
#include <QHBoxLayout>

PlayerListItem::PlayerListItem(const QColor& color, const QString& playerName, QWidget* parent)
    : FloatingPanel(parent) {

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 6, 10, 6);
    layout->setSpacing(0);

    auto* dotText = new DotAndText(color, playerName, this);

    layout->addWidget(dotText);
    layout->addStretch();
}
