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

    // 业障因果系统
    void setYezhang(double yezhang);          // 当前业障值
    double yezhangVolatilityBoost() const;   // 业障导致的额外波动率
    bool isTribulationActive() const;         // 是否正在经历天劫

    // 天劫事件
    void triggerTribulation();                // 触发天劫

signals:
    void eventChanged(const QString& event);
    void tribulationTriggered(const QString& desc); // 天劫触发信号

private:
    QString m_currentEvent;
    double m_driftModifier;
    double m_volatilityModifier;
    double m_duration;
    double m_remainingTime;
    double m_yezhang;          // 当前业障值
    bool m_tribulationActive;  // 天劫状态
    double m_tribulationTimer; // 天劫持续时间

    struct EventData {
        QString description;
        double drift;
        double volatility;
        double duration;
        bool isNegative;  // 是否为负面事件
    };

    QList<EventData> m_events;

    void applyYezhangToEvents();  // 根据业障调整事件参数
};

#endif // MARKETEVENT_H
