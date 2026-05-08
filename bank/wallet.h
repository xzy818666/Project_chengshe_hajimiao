#ifndef WALLET_H
#define WALLET_H

#include <QObject>
#include <QMap>
#include <QDateTime>

class Wallet : public QObject
{
    Q_OBJECT
public:
    explicit Wallet(QObject *parent = nullptr);

    double merit() const;
    void earn(double amount);
    bool spend(double amount);

    double totalDebt() const;
    double creditScore() const;
    double maxBorrow() const;
    bool borrow(double amount);
    bool repay(double amount);

    double yezhang() const;
    void addYezhang(double amount);
    void reduceYezhang(double amount);

    double efficiency() const;

    // 通货膨胀
    double dailyInflationRate() const;
    void setDailyInflationRate(double rate);
    void applyInflation(double dt);

    void updateDebtInterest();

    // 资产持仓
    void addAsset(const QString& assetId, double shares);
    double getAssetShares(const QString& assetId) const;
    bool removeAsset(const QString& assetId, double shares);
    const QMap<QString, double>& assets() const;

    // 成本基础与交易记录（成就系统用）
    void recordAssetBuy(const QString& assetId, double shares, double cost);
    void recordAssetSell(const QString& assetId, double shares, double revenue);

    // 定期存款
    struct FixedDeposit {
        double amount;
        QDateTime dueDate;
        double rate;
        QString id;
    };
    QString addFixedDeposit(double amount, int days, double rate);
    bool withdrawFixedDeposit(const QString& id);
    const QList<FixedDeposit>& fixedDeposits() const;

    // 活期利息
    double savingsBalance() const;
    void addSavings(double amount);
    bool withdrawSavings(double amount);
    void updateSavingsInterest();

signals:
    void meritChanged(double value);
    void debtChanged(double value);
    void yezhangChanged(double value);
    void assetsChanged();
    void assetBought(const QString& assetId, double shares, double cost);
    void assetSold(const QString& assetId, double shares, double revenue, double profit);
    void borrowed(double amount);
    void repaid(double amount);

private:
    double m_merit;
    double m_savings;
    double m_debt;
    double m_yezhang;
    double m_creditScore;
    double m_dailyInflationRate;

    QMap<QString, double> m_assets;
    QMap<QString, double> m_assetCostBasis;
    QList<FixedDeposit> m_fixedDeposits;

    static const double SAVINGS_RATE;
    static const double LOAN_RATE;
    static const double YEZHANG_CONVERSION_RATE;
    static const double DEFAULT_DAILY_INFLATION;
};

#endif // WALLET_H
