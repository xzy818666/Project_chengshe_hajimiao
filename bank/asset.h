#ifndef ASSET_H
#define ASSET_H

#include <QObject>
#include <QVector>

class Asset : public QObject
{
    Q_OBJECT
public:
    enum RiskLevel {
        Low,
        MediumLow,
        Medium,
        High
    };

    explicit Asset(QObject *parent = nullptr);

    virtual QString name() const = 0;
    virtual QString id() const = 0;
    virtual double price() const = 0;
    virtual RiskLevel riskLevel() const = 0;
    virtual double calculateReturn(double shares, double dt) const;

    virtual void update(double dt, double marketDrift = 0, double marketVolatility = 1) = 0;

    const QVector<double>& priceHistory() const;
    double change24h() const;

signals:
    void priceChanged(double price);

protected:
    QVector<double> m_priceHistory;
    static const int HISTORY_SIZE;
};

#endif // ASSET_H
