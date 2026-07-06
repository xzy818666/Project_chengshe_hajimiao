#ifndef CULTIVATIONLEVEL_H
#define CULTIVATIONLEVEL_H

#include <QObject>
#include <QString>

/**
 * 修炼等级系统 — 核心机制之一
 * 每次点击法器获取经验，升级后提升全局属性
 * 与法器共鸣、业障因果形成有机循环
 */
class CultivationLevel : public QObject
{
    Q_OBJECT
public:
    explicit CultivationLevel(QObject *parent = nullptr);

    int level() const;
    int exp() const;
    int maxExp() const;
    double progress() const; // 0.0 ~ 1.0

    // 升级奖励属性
    double efficiencyBonus() const;   // 每级 +2% 基础效率
    double critRateBonus() const;     // 每级 +1% 暴击率上限
    double maxLeverageBonus() const;  // 每级 +0.1× 最大杠杆
    double autoIncomeBonus() const;   // 每级 +3% 自动收益

    // 经验获取与升级
    void addExp(int amount);
    void setLevel(int level); // 用于存档/读档
    void setExp(int exp);

    QString levelTitle() const; // 等级称号

signals:
    void levelUp(int newLevel, QString title);
    void expChanged(int current, int max);

private:
    int m_level;
    int m_exp;
    int m_maxExp;

    int calculateMaxExp(int level) const;
    void checkLevelUp();
};

#endif // CULTIVATIONLEVEL_H
