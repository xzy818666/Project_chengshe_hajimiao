#ifndef MARKETEVENT_H
#define MARKETEVENT_H

#include <QObject>
#include <QString>

class MarketEvent : public QObject
{
    Q_OBJECT
public:
    explicit MarketEvent(QObject *parent = nullptr);

    QString currentEvent() const;
    double driftModifier() const;
    double volatilityModifier() const;

    void triggerRandomEvent();
    void update(double dt);
    bool isActive() const;

signals:
    void eventChanged(const QString& event);

private:
    QString m_currentEvent;
    double m_driftModifier;
    double m_volatilityModifier;
    double m_duration;
    double m_remainingTime;

    struct EventData {
        QString description;
        double drift;
        double volatility;
        double duration;
    };

    QList<EventData> m_events;
};

#endif // MARKETEVENT_H
