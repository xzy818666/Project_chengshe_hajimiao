#include "marketevent.h"
#include <cstdlib>

MarketEvent::MarketEvent(QObject *parent) : QObject(parent),
    m_driftModifier(0),
    m_volatilityModifier(1),
    m_duration(0),
    m_remainingTime(0)
{
    m_events = {
        {"期末祈福潮，指数 +15%", 15, 1.2, 30},
        {"体测取消，掀起退市潮", -10, 1.5, 20},
        {"官方辟谣反涌", 8, 1.3, 15},
        {"期中考试临近，功德需求上升", 10, 1.1, 40},
        {"周末休市", 0, 0.3, 60},
        {"佛系行情，波动收窄", 2, 0.5, 30},
        {"政策利好，全面上涨", 12, 1.1, 25},
        {"恐慌抛售", -15, 2.0, 15},
        {"外资入场", 8, 1.0, 20},
        {"加息预期", -5, 1.4, 25},
        {"降息利好", 10, 1.2, 30},
        {"财报季来临", 5, 1.5, 40}
    };
}

QString MarketEvent::currentEvent() const
{
    return m_currentEvent;
}

double MarketEvent::driftModifier() const
{
    return m_driftModifier;
}

double MarketEvent::volatilityModifier() const
{
    return m_volatilityModifier;
}

void MarketEvent::triggerRandomEvent()
{
    int idx = rand() % m_events.size();
    EventData event = m_events[idx];
    m_currentEvent = event.description;
    m_driftModifier = event.drift;
    m_volatilityModifier = event.volatility;
    m_duration = event.duration;
    m_remainingTime = event.duration;

    emit eventChanged(m_currentEvent);
}

void MarketEvent::update(double dt)
{
    if (m_remainingTime > 0) {
        m_remainingTime -= dt;
        if (m_remainingTime <= 0) {
            m_currentEvent = "";
            m_driftModifier = 0;
            m_volatilityModifier = 1;
            emit eventChanged(m_currentEvent);
        }
    } else if (rand() % 200 == 0) {
        triggerRandomEvent();
    }
}

bool MarketEvent::isActive() const
{
    return m_remainingTime > 0;
}
