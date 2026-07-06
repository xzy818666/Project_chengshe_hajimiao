#include "achievementmanager.h"
#include <QtMath>

AchievementManager::AchievementManager(QObject *parent)
    : QObject(parent)
{
    m_titles["first_step"] = "初入佛门";
    m_descriptions["first_step"] = "功德达到 100";

    m_titles["click_master"] = "腱鞘炎警告";
    m_descriptions["click_master"] = "单次会话点击法器 1000 次";

    m_titles["wall_street"] = "华尔街之狼";
    m_descriptions["wall_street"] = "累计实现盈利 5000 功德";

    m_titles["debt_king"] = "负债累累";
    m_descriptions["debt_king"] = "累计借贷达到 10000 功德";

    m_titles["infinite_merit"] = "功德无量";
    m_descriptions["infinite_merit"] = "净资产达到 100000";

    m_titles["rooftop"] = "天台见";
    m_descriptions["rooftop"] = "单次交易亏损达到 50% 或持仓浮亏 50%";

    m_titles["diversified"] = "资产配置大师";
    m_descriptions["diversified"] = "同时持有 3 种以上资产";

    m_titles["leverage_gambler"] = "杠杆赌徒";
    m_descriptions["leverage_gambler"] = "持有轮回孽缘期货";
}

bool AchievementManager::isUnlocked(const QString& id) const
{
    return m_unlocked.contains(id);
}

QString AchievementManager::title(const QString& id) const
{
    return m_titles.value(id, id);
}

QString AchievementManager::description(const QString& id) const
{
    return m_descriptions.value(id, "");
}

QStringList AchievementManager::allIds() const
{
    return m_titles.keys();
}

void AchievementManager::unlock(const QString& id)
{
    if (!m_unlocked.contains(id)) {
        m_unlocked.insert(id);
        emit achievementUnlocked(title(id), description(id));
    }
}

void AchievementManager::onMeritChanged(double merit)
{
    m_maxMerit = qMax(m_maxMerit, merit);
    if (merit >= 100.0) unlock("first_step");
}

void AchievementManager::onNetWorthChanged(double netWorth)
{
    m_maxNetWorth = qMax(m_maxNetWorth, netWorth);
    if (netWorth >= 100000.0) unlock("infinite_merit");
}

void AchievementManager::onAssetBought(const QString& assetId, double shares, double cost)
{
    auto& rec = m_assetRecords[assetId];
    rec.shares += shares;
    rec.totalCost += cost;
}

void AchievementManager::onAssetSold(const QString& assetId, double shares, double revenue, double profit)
{
    auto& rec = m_assetRecords[assetId];
    rec.shares = qMax(0.0, rec.shares - shares);
    if (rec.shares <= 0.0) {
        rec.totalCost = 0;
    } else {
        double avgCost = rec.totalCost / (rec.shares + shares);
        rec.totalCost = avgCost * rec.shares;
    }

    m_totalRealizedProfit += profit;
    if (m_totalRealizedProfit >= 5000.0) unlock("wall_street");

    double costBasis = revenue - profit;
    if (costBasis > 0 && profit < -costBasis * 0.5) unlock("rooftop");
}

void AchievementManager::onBorrowed(double amount)
{
    m_totalBorrowed += amount;
    if (m_totalBorrowed >= 10000.0) unlock("debt_king");
}

void AchievementManager::onYezhangChanged(double yezhang)
{
    Q_UNUSED(yezhang);
}

void AchievementManager::onClickReport(int sessionClicks)
{
    m_maxSessionClicks = qMax(m_maxSessionClicks, sessionClicks);
    if (m_maxSessionClicks >= 1000) unlock("click_master");
}

void AchievementManager::checkHoldings(const QMap<QString, double>& assets)
{
    int holdingCount = 0;
    for (auto it = assets.begin(); it != assets.end(); ++it) {
        if (it.value() > 0) ++holdingCount;
    }
    if (holdingCount >= 3) unlock("diversified");
    if (assets.contains("samsara_futures") && assets["samsara_futures"] > 0.0) {
        unlock("leverage_gambler");
    }
}

void AchievementManager::checkPortfolioLoss(const QMap<QString, double>& assets, const QMap<QString, double>& prices)
{
    for (auto it = assets.begin(); it != assets.end(); ++it) {
        QString id = it.key();
        double shares = it.value();
        if (shares <= 0) continue;
        if (!m_assetRecords.contains(id)) continue;
        const AssetRecord& rec = m_assetRecords[id];
        if (rec.totalCost <= 0) continue;
        double price = prices.value(id, 0);
        double value = shares * price;
        if (value < rec.totalCost * 0.5) {
            unlock("rooftop");
        }
    }
}
