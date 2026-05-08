#include "wallet.h"

const double Wallet::SAVINGS_RATE = 0.05;
const double Wallet::LOAN_RATE = 0.01;
const double Wallet::YEZHANG_CONVERSION_RATE = 0.10;

Wallet::Wallet(QObject *parent) : QObject(parent),
    m_merit(1000),
    m_savings(0),
    m_debt(0),
    m_yezhang(0),
    m_creditScore(500)
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
