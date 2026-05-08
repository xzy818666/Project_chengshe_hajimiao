#ifndef PORTFOLIOADVISOR_H
#define PORTFOLIOADVISOR_H

#include <QObject>
#include <QList>
#include <QString>

class Asset;

class PortfolioAdvisor : public QObject
{
    Q_OBJECT
public:
    struct Advice {
        QString assetId;
        QString assetName;
        double weight;
        double expectedReturn;
        double risk;
        QString reason;
    };

    explicit PortfolioAdvisor(QObject *parent = nullptr);

    QList<Advice> advise(const QList<Asset*>& assets) const;

private:
    double calculateSharpe(const QVector<double>& history) const;
};

#endif // PORTFOLIOADVISOR_H
