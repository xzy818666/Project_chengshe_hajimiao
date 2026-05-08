#include "marketevent.h"
#include <cstdlib>

MarketEvent::MarketEvent(QObject *parent) : QObject(parent),
    m_driftModifier(0),
    m_volatilityModifier(1),
    m_duration(0),
    m_remainingTime(0)
{
    m_events = {
        {"期末祈福潮，指数 +15%", 5, 1.2, 30},
        {"体测取消，掀起退市潮", -5, 1.5, 20},
        {"官方辟谣反涌", 3, 1.3, 15},
        {"期中考试临近，功德需求上升", 3, 1.1, 40},
        {"周末休市", 0, 0.3, 60},
        {"佛系行情，波动收窄", 2, 0.5, 30},
        {"政策利好，全面上涨", 2, 1.1, 25},
        {"恐慌抛售", -10, 2.0, 15},
        {"外资入场", 4, 1.0, 20},
        {"加息预期", -2, 1.4, 25},
        {"降息利好", 3, 1.2, 30},
        {"财报季来临", 3, 1.5, 40},
        {"食堂新开光素斋窗口，功德食品板块大涨", 4, 1.2, 25},
        {"宿舍楼断网，线上诵经平台崩盘", -6, 1.8, 20},
        {"校花转发锦鲤，社交媒体功德概念炒作", 5, 1.5, 15},
        {"教务处严查代敲木鱼，灰色产业受挫", -4, 1.3, 20},
        {"大师讲座：未来十年功德经济展望", 2, 0.8, 35},
        {"AI木鱼被曝存在后门，量子佛珠概念突起", 3, 1.6, 20},
        {"毕业季功德兑换学分政策出台", 6, 1.4, 30},
        {"校方宣布功德不可用于抵扣挂科", -5, 1.7, 15},
        {"神秘土豪一次性捐赠百万功德", 4, 1.1, 20},
        {"功德矿机被曝耗电过高，环保组织抗议", -3, 1.5, 25},
        {"元宇宙佛堂上线，虚拟功德交易量激增", 5, 1.3, 25},
        {"系统维护，所有资产暂停交易", 0, 0.1, 10},
        {"校园贷新规：下辈子借款需双签", -2, 1.2, 20},
        {"佛法chatGPT通过图灵测试，自动祈福合法化", 4, 1.4, 25},
        {"台风天停课，宿舍集体念经推高指数", 3, 1.0, 30},
        {"二手木鱼市场泡沫破裂", -4, 1.6, 18},
        {"考古发现远古佛经， antique功德NFT火爆", 5, 1.8, 22}
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
