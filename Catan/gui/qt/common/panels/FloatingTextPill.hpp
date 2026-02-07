//
// Created by matija on 2/5/26.
//

#ifndef CATAN_FLOATINGTEXTPILL_HPP
#define CATAN_FLOATINGTEXTPILL_HPP

#include <QLabel>
#include <QPointer>
#include <common/panels/FloatingPanel.h>

class FloatingTextPill : public FloatingPanel {
    Q_OBJECT
public:
    explicit FloatingTextPill(QWidget* parent = nullptr);

public slots:
    void setText(const QString& text);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void reposition();

    QLabel* m_label;
    int m_margin = 12;
};
#endif //CATAN_FLOATINGTEXTPILL_HPP