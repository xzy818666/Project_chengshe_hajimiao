#include "marketevent.h"
#include <cstdlib>
#include <cmath>

MarketEvent::MarketEvent(QObject *parent) : QObject(parent),
    m_driftModifier(0),
    m_volatilityModifier(1),
    m_duration(0),
    m_remainingTime(0),
    m_yezhang(0),
    m_tribulationActive(false),
    m_tribulationTimer(0)
{
    m_events = {
        {"期末祈福潮，指数 +15%", 5, 1.2, 30, false},
        {"体测取消，掀起退市潮", -5, 1.5, 20, true},
        {"官方辟谣反涌", 3, 1.3, 15, false},
        {"期中考试临近，功德需求上升", 3, 1.1, 40, false},
        {"周末休市", 0, 0.3, 60, false},
        {"佛系行情，波动收窄", 2, 0.5, 30, false},
        {"政策利好，全面上涨", 2, 1.1, 25, false},
        {"恐慌抛售", -10, 2.0, 15, true},
        {"外资入场", 4, 1.0, 20, false},
        {"加息预期", -2, 1.4, 25, true},
        {"降息利好", 3, 1.2, 30, false},
        {"财报季来临", 3, 1.5, 40, false},
        {"食堂新开光素斋窗口，功德食品板块大涨", 4, 1.2, 25, false},
        {"宿舍楼断网，线上诵经平台崩盘", -6, 1.8, 20, true},
        {"校花转发锦鲤，社交媒体功德概念炒作", 5, 1.5, 15, false},
        {"教务处严查代敲木鱼，灰色产业受挫", -4, 1.3, 20, true},
        {"大师讲座：未来十年功德经济展望", 2, 0.8, 35, false},
        {"AI木鱼被曝存在后门，量子佛珠概念突起", 3, 1.6, 20, false},
        {"毕业季功德兑换学分政策出台", 6, 1.4, 30, false},
        {"校方宣布功德不可用于抵扣挂科", -5, 1.7, 15, true},
        {"神秘土豪一次性捐赠百万功德", 4, 1.1, 20, false},
        {"功德矿机被曝耗电过高，环保组织抗议", -3, 1.5, 25, true},
        {"元宇宙佛堂上线，虚拟功德交易量激增", 5, 1.3, 25, false},
        {"系统维护，所有资产暂停交易", 0, 0.1, 10, false},
        {"校园贷新规：下辈子借款需双签", -2, 1.2, 20, true},
        {"佛法chatGPT通过图灵测试，自动祈福合法化", 4, 1.4, 25, false},
        {"台风天停课，宿舍集体念经推高指数", 3, 1.0, 30, false},
        {"二手木鱼市场泡沫破裂", -4, 1.6, 18, true},
        {"考古发现远古佛经， antique功德NFT火爆", 5, 1.8, 22, false}
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

void MarketEvent::setYezhang(double yezhang)
{
    m_yezhang = yezhang;
}

double MarketEvent::yezhangVolatilityBoost() const
{
    // 业障每增加 100，波动率增加 5%
    return 1.0 + (m_yezhang / 100.0) * 0.05;
}

bool MarketEvent::isTribulationActive() const
{
    return m_tribulationActive;
}

void MarketEvent::triggerTribulation()
{
    m_tribulationActive = true;
    m_tribulationTimer = 20; // 天劫持续 20 秒
    m_currentEvent = "⚡ 天劫降临！业障反噬，所有资产价格腰斩！⚡";
    m_driftModifier = -15; // 极端下跌
    m_volatilityModifier = 5.0; // 极高波动率
    m_duration = 20;
    m_remainingTime = 20;
    emit eventChanged(m_currentEvent);
    emit tribulationTriggered("天劫降临！业障反噬导致全市场崩盘！");
}

void MarketEvent::triggerRandomEvent()
{
    // 业障影响：高业障更容易触发负面事件
    QList<int> candidates;
    for (int i = 0; i < m_events.size(); ++i) {
        // 业障越高，负面事件权重越大
        if (m_events[i].isNegative && m_yezhang > 200) {
            int weight = 1 + static_cast<int>(m_yezhang / 200);
            for (int w = 0; w < weight; ++w) {
                candidates.append(i);
            }
        } else {
            candidates.append(i);
        }
    }

    int idx = candidates[rand() % candidates.size()];
    EventData event = m_events[idx];
    m_currentEvent = event.description;
    m_driftModifier = event.drift;
    m_volatilityModifier = event.volatility * yezhangVolatilityBoost();
    m_duration = event.duration;
    m_remainingTime = event.duration;

    emit eventChanged(m_currentEvent);
}

void MarketEvent::update(double dt)
{
    // 天劫处理
    if (m_tribulationActive) {
        m_tribulationTimer -= dt;
        if (m_tribulationTimer <= 0) {
            m_tribulationActive = false;
            m_currentEvent = "";
            m_driftModifier = 0;
            m_volatilityModifier = 1;
            emit eventChanged(m_currentEvent);
        }
        return;
    }

    // 检查是否触发天劫：业障 >= 1000 且随机触发
    if (m_yezhang >= 1000 && rand() % 500 == 0) {
        triggerTribulation();
        return;
    }

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
    return m_remainingTime > 0 || m_tribulationActive;
}
