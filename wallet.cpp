#include "wallet.h"

const double Wallet::SAVINGS_RATE = 0.05;
const double Wallet::LOAN_RATE = 0.01;
const double Wallet::YEZHANG_CONVERSION_RATE = 0.10;
const double Wallet::DEFAULT_DAILY_INFLATION = 0.01; // 每日通胀 1%

Wallet::Wallet(QObject *parent) : QObject(parent),
    m_merit(1000),
    m_savings(0),
    m_debt(0),
    m_yezhang(0),
    m_creditScore(500),
    m_dailyInflationRate(DEFAULT_DAILY_INFLATION),
    m_nextLifeMeritPool(1000),
    m_nextLifeLoss(0)
{
}

Wallet::Wallet(QObject *parent, double initialMerit, double nextLifePool,
               double inflationRate, double creditScore)
    : QObject(parent),
      m_merit(initialMerit),
      m_savings(0),
      m_debt(0),
      m_yezhang(0),
      m_creditScore(creditScore),
      m_dailyInflationRate(inflationRate),
      m_nextLifeMeritPool(nextLifePool),
      m_nextLifeLoss(0)
{
}

double Wallet::merit() const
{
    return m_merit;
}

void Wallet::earn(double amount)
{
    m_merit += amount;
    emit meritChanged(m_merit);
}

bool Wallet::spend(double amount)
{
    if (m_merit >= amount) {
        m_merit -= amount;
        emit meritChanged(m_merit);
        return true;
    }
    return false;
}

double Wallet::totalDebt() const
{
    return m_debt;
}

double Wallet::creditScore() const
{
    return m_creditScore;
}

double Wallet::maxBorrow() const
{
    return m_creditScore * 2;
}

bool Wallet::borrow(double amount)
{
    if (m_debt + amount <= maxBorrow()) {
        m_debt += amount;
        m_merit += amount;
        m_creditScore = qMax(0.0, m_creditScore - amount * 0.1);
        emit debtChanged(m_debt);
        emit meritChanged(m_merit);
        emit borrowed(amount);
        return true;
    }
    return false;
}

bool Wallet::repay(double amount)
{
    if (m_merit >= amount) {
        m_merit -= amount;
        m_debt = qMax(0.0, m_debt - amount);
        m_creditScore = qMin(1000.0, m_creditScore + amount * 0.05);
        emit debtChanged(m_debt);
        emit meritChanged(m_merit);
        emit repaid(amount);
        return true;
    }
    return false;
}

double Wallet::yezhang() const
{
    return m_yezhang;
}

void Wallet::addYezhang(double amount)
{
    m_yezhang += amount;
    emit yezhangChanged(m_yezhang);
}

void Wallet::reduceYezhang(double amount)
{
    if (m_merit >= amount) {
        m_merit -= amount;
        m_yezhang = qMax(0.0, m_yezhang - amount);
        emit meritChanged(m_merit);
        emit yezhangChanged(m_yezhang);
    }
}

double Wallet::efficiency() const
{
    return 1.0 * qMax(0.5, 1.0 - m_yezhang / 2000.0);
}

double Wallet::dailyInflationRate() const
{
    return m_dailyInflationRate;
}

void Wallet::setDailyInflationRate(double rate)
{
    m_dailyInflationRate = qMax(0.0, rate);
}

void Wallet::applyInflation(double dt)
{
    if (m_dailyInflationRate <= 0 || dt <= 0) return;

    // 游戏内一天压缩为1秒，dt单位是秒
    const double DAY_SECONDS = 1.0;
    double tickRate = m_dailyInflationRate * (dt / DAY_SECONDS);
    double factor = 1.0 - tickRate;
    factor = qMax(0.0, factor);

    double meritBefore = m_merit;
    double savingsBefore = m_savings;

    m_merit *= factor;
    m_savings *= factor;

    if (!qFuzzyCompare(meritBefore, m_merit)) {
        emit meritChanged(m_merit);
    }
    if (!qFuzzyCompare(savingsBefore, m_savings)) {
        // 储蓄变化通过 meritChanged 间接提示，或以后加单独信号
    }
}

void Wallet::updateDebtInterest()
{
    if (m_debt > 0) {
        double interest = m_debt * LOAN_RATE / 86400.0;
        m_debt += interest;

        if (m_debt > maxBorrow() * 1.5) {
            double excess = m_debt - maxBorrow() * 1.5;
            double yezhangAdded = excess * YEZHANG_CONVERSION_RATE;
            m_debt -= yezhangAdded;
            addYezhang(yezhangAdded);
        }

        emit debtChanged(m_debt);
    }
}

void Wallet::addAsset(const QString& assetId, double shares)
{
    m_assets[assetId] += shares;
    emit assetsChanged();
}

double Wallet::getAssetShares(const QString& assetId) const
{
    return m_assets.value(assetId, 0);
}

bool Wallet::removeAsset(const QString& assetId, double shares)
{
    if (m_assets.value(assetId, 0) >= shares) {
        m_assets[assetId] -= shares;
        if (m_assets[assetId] <= 0) {
            m_assets.remove(assetId);
        }
        emit assetsChanged();
        return true;
    }
    return false;
}

const QMap<QString, double>& Wallet::assets() const
{
    return m_assets;
}

void Wallet::recordAssetBuy(const QString& assetId, double shares, double cost)
{
    m_assetCostBasis[assetId] += cost;
    emit assetBought(assetId, shares, cost);
}

void Wallet::recordAssetSell(const QString& assetId, double shares, double revenue)
{
    double profit = 0;
    if (m_assetCostBasis.contains(assetId)) {
        double totalSharesBefore = m_assets.value(assetId, 0) + shares;
        double costBasis = 0;
        if (totalSharesBefore > 0) {
            double avgCost = m_assetCostBasis[assetId] / totalSharesBefore;
            costBasis = avgCost * shares;
        }
        m_assetCostBasis[assetId] -= costBasis;
        if (m_assetCostBasis[assetId] <= 0) {
            m_assetCostBasis.remove(assetId);
        }
        profit = revenue - costBasis;
    }
    emit assetSold(assetId, shares, revenue, profit);
}

QString Wallet::addFixedDeposit(double amount, int days, double rate)
{
    if (m_merit >= amount) {
        m_merit -= amount;
        FixedDeposit fd;
        fd.amount = amount;
        fd.dueDate = QDateTime::currentDateTime().addDays(days);
        fd.rate = rate;
        fd.id = QString("FD%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
        m_fixedDeposits.append(fd);
        emit meritChanged(m_merit);
        return fd.id;
    }
    return QString();
}

bool Wallet::withdrawFixedDeposit(const QString& id)
{
    for (auto it = m_fixedDeposits.begin(); it != m_fixedDeposits.end(); ++it) {
        if (it->id == id) {
            bool isDue = QDateTime::currentDateTime() >= it->dueDate;
            double amount = it->amount;
            if (isDue) {
                amount *= (1 + it->rate);
            } else {
                amount *= 0.95;
            }
            m_merit += amount;
            m_fixedDeposits.erase(it);
            emit meritChanged(m_merit);
            return true;
        }
    }
    return false;
}

const QList<Wallet::FixedDeposit>& Wallet::fixedDeposits() const
{
    return m_fixedDeposits;
}

double Wallet::savingsBalance() const
{
    return m_savings;
}

void Wallet::addSavings(double amount)
{
    if (m_merit >= amount) {
        m_merit -= amount;
        m_savings += amount;
        emit meritChanged(m_merit);
    }
}

bool Wallet::withdrawSavings(double amount)
{
    if (m_savings >= amount) {
        m_savings -= amount;
        m_merit += amount;
        emit meritChanged(m_merit);
        return true;
    }
    return false;
}

void Wallet::updateSavingsInterest()
{
    double interest = m_savings * SAVINGS_RATE / 86400.0;
    m_savings += interest;
}

double Wallet::nextLifeMeritPool() const
{
    return m_nextLifeMeritPool;
}

double Wallet::nextLifeLoss() const
{
    return m_nextLifeLoss;
}

void Wallet::applySamsaraLoss(double lossAmount)
{
    if (lossAmount <= 0) return;
    double fromPool = qMin(lossAmount, m_nextLifeMeritPool);
    m_nextLifeMeritPool -= fromPool;
    m_nextLifeLoss += fromPool;

    double remaining = lossAmount - fromPool;
    if (remaining > 0) {
        m_debt += remaining;
        emit debtChanged(m_debt);
    }
}

double Wallet::getAssetCostBasis(const QString& assetId) const
{
    return m_assetCostBasis.value(assetId, 0);
}

void Wallet::clearCostBasis(const QString& assetId)
{
    m_assetCostBasis.remove(assetId);
}
