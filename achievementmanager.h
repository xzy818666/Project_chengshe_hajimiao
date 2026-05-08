#ifndef ACHIEVEMENTMANAGER_H
#define ACHIEVEMENTMANAGER_H

#include <QObject>
#include <QSet>
#include <QMap>
#include <QStringList>

class AchievementManager : public QObject
{
    Q_OBJECT
public:
    explicit AchievementManager(QObject *parent = nullptr);

    bool isUnlocked(const QString& id) const;
    QString title(const QString& id) const;
    QString description(const QString& id) const;
    QStringList allIds() const;

public slots:
    void onMeritChanged(double merit);
    void onNetWorthChanged(double netWorth);
    void onAssetBought(const QString& assetId, double shares, double cost);
    void onAssetSold(const QString& assetId, double shares, double revenue, double profit);
    void onBorrowed(double amount);
    void onYezhangChanged(double yezhang);
    void onClickReport(int sessionClicks);
    void checkHoldings(const QMap<QString, double>& assets);
    void checkPortfolioLoss(const QMap<QString, double>& assets, const QMap<QString, double>& prices);

signals:
    void achievementUnlocked(const QString& title, const QString& description);

private:
    void unlock(const QString& id);

    QSet<QString> m_unlocked;
    QMap<QString, QString> m_titles;
    QMap<QString, QString> m_descriptions;

    double m_maxMerit = 0;
    double m_maxNetWorth = 0;
    double m_totalBorrowed = 0;
    double m_totalRealizedProfit = 0;
    int m_maxSessionClicks = 0;

    struct AssetRecord {
        double shares = 0;
        double totalCost = 0;
    };
    QMap<QString, AssetRecord> m_assetRecords;
};

#endif // ACHIEVEMENTMANAGER_H
