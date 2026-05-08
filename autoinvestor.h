#ifndef AUTOINVESTOR_H
#define AUTOINVESTOR_H

#include <QObject>
#include <QList>
#include <QString>

class Wallet;
class Asset;
class PortfolioAdvisor;

class AutoInvestor : public QObject
{
    Q_OBJECT
public:
    explicit AutoInvestor(QObject *parent = nullptr);

    void setEnabled(bool enabled);
    void setIntervalSeconds(int seconds);
    void setAmountPerInvest(double amount);
    void setTargetAssetId(const QString& assetId); // empty = follow advisor

    void update(double dt, Wallet* wallet, const QList<Asset*>& assets, PortfolioAdvisor* advisor = nullptr);

signals:
    void invested(const QString& assetName, double amount);
    void logMessage(const QString& msg);

private:
    bool m_enabled;
    int m_interval;
    double m_amount;
    QString m_targetAssetId;
    double m_accumulated;
};

#endif // AUTOINVESTOR_H
