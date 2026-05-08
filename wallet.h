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

    void updateDebtInterest();

    // 资产持仓
    void addAsset(const QString& assetId, double shares);
    double getAssetShares(const QString& assetId) const;
    bool removeAsset(const QString& assetId, double shares);
    const QMap<QString, double>& assets() const;

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

private:
    double m_merit;
    double m_savings;
    double m_debt;
    double m_yezhang;
    double m_creditScore;

    QMap<QString, double> m_assets;
    QList<FixedDeposit> m_fixedDeposits;

    static const double SAVINGS_RATE;
    static const double LOAN_RATE;
    static const double YEZHANG_CONVERSION_RATE;
};

#endif // WALLET_H
