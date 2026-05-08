#include "autoinvestor.h"
#include "wallet.h"
#include "asset.h"
#include "portfolioadvisor.h"
#include <QtMath>

AutoInvestor::AutoInvestor(QObject *parent)
    : QObject(parent)
    , m_enabled(false)
    , m_interval(60)
    , m_amount(100.0)
    , m_accumulated(0.0)
{
}

void AutoInvestor::setEnabled(bool enabled)
{
    m_enabled = enabled;
    if (!enabled) m_accumulated = 0.0;
}

void AutoInvestor::setIntervalSeconds(int seconds)
{
    m_interval = qMax(1, seconds);
}

void AutoInvestor::setAmountPerInvest(double amount)
{
    m_amount = qMax(1.0, amount);
}

void AutoInvestor::setTargetAssetId(const QString& assetId)
{
    m_targetAssetId = assetId;
}

void AutoInvestor::update(double dt, Wallet* wallet, const QList<Asset*>& assets, PortfolioAdvisor* advisor)
{
    if (!m_enabled || !wallet || assets.isEmpty()) return;

    m_accumulated += dt;
    if (m_accumulated < m_interval) return;
    m_accumulated = 0.0;

    if (wallet->merit() < m_amount) {
        emit logMessage("定投失败: 功德不足");
        return;
    }

    Asset* target = nullptr;

    if (!m_targetAssetId.isEmpty()) {
        for (Asset* a : assets) {
            if (a->id() == m_targetAssetId) {
                target = a;
                break;
            }
        }
    } else if (advisor) {
        auto advice = advisor->advise(assets);
        double maxWeight = -1.0;
        for (const auto& a : advice) {
            for (Asset* asset : assets) {
                if (asset->id() == a.assetId && a.weight > maxWeight) {
                    maxWeight = a.weight;
                    target = asset;
                }
            }
        }
    }

    if (!target && !assets.isEmpty()) {
        target = assets.first();
    }

    if (!target) {
        emit logMessage("定投失败: 未找到目标资产");
        return;
    }

    double price = target->price();
    if (price <= 0) {
        emit logMessage("定投失败: 价格异常");
        return;
    }

    double shares = m_amount / price;
    wallet->spend(m_amount);
    wallet->addAsset(target->id(), shares);
    emit invested(target->name(), m_amount);
    emit logMessage(QString("自动定投: 花费 %1 功德买入 %2 %3 份")
                    .arg(m_amount, 0, 'f', 2).arg(target->name()).arg(shares, 0, 'f', 4));
}
