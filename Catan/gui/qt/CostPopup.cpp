#include "CostPopup.h"
CostPopup::CostPopup(QWidget* parent)
    : FloatingPanel(Qt::ToolTip,parent)
{
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(R"(
        QFrame {
            background: white;
            border-radius: 10px;
            border: 1px solid #ccc;
        }
    )");

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 8, 10, 8);

    m_row = new QCardRow(this);

    layout->addWidget(m_row);

}
void CostPopup::refresh()
{
    m_row->clear();
    for(ResourceType resource : MoveCosts::costFor(m_action))
        m_row->addCard(CardSpec({CardKind::Resource, CardFace::FaceUp, resource}));
    adjustSize();
}

