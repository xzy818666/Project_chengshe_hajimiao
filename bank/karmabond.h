#ifndef KARMABOND_H
#define KARMABOND_H

#include "asset.h"

class KarmaBond : public Asset
{
    Q_OBJECT
public:
    explicit KarmaBond(QObject *parent = nullptr);

    QString name() const override;
    QString id() const override;
    double price() const override;
    RiskLevel riskLevel() const override;

    void update(double dt, double marketDrift = 0, double marketVolatility = 1) override;

private:
    static const double INTEREST_RATE;
};

#endif // KARMABOND_H
