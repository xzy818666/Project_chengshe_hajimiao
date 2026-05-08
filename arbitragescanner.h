#ifndef ARBITRAGESCANNER_H
#define ARBITRAGESCANNER_H

#include <QObject>
#include <QList>
#include <QString>

class Asset;

class ArbitrageScanner : public QObject
{
    Q_OBJECT
public:
    struct Opportunity {
        QString assetId;
        QString description;
        double deviation;
    };

    explicit ArbitrageScanner(QObject *parent = nullptr);

    QList<Opportunity> scan(const QList<Asset*>& assets) const;

private:
    Asset* findAsset(const QList<Asset*>& assets, const QString& id) const;
};

#endif // ARBITRAGESCANNER_H
