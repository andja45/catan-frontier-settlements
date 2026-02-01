//
// Created by matija on 2/1/26.
//

#ifndef CATAN_PULSESTATE_HPP
#define CATAN_PULSESTATE_HPP
#include <QColor>
#include <QObject>
#include <QTimer>

class PulseState : public QObject {
    Q_OBJECT
public:
    explicit PulseState(QObject* parent = nullptr)
        : QObject(parent)
    {
        m_timer.setInterval(16);
        connect(&m_timer, &QTimer::timeout, this, &PulseState::tick);
        m_timer.start();
    }
    QColor pulseColor(QColor base) {
        float t = m_value;
        auto col=base.lighter(100 + int(80 * t));
        col.setAlphaF(0.6 + 0.4 * t);
        return col;
    }


    double value() const { return m_value; }

    signals:
        void changed();

private:
    void tick() {
        m_phase += 0.08;
        m_value = 0.5 + 0.5 * std::sin(m_phase);
        emit changed();
    }

    QTimer m_timer;
    double m_phase = 0.0;
    double m_value = 0.0;
};


#endif //CATAN_PULSESTATE_HPP