#ifndef INSTRUMENTRESONANCE_H
#define INSTRUMENTRESONANCE_H

#include <QObject>
#include <QList>
#include <QString>
#include "instrument.h"

/**
 * 法器组合共鸣系统 — 核心技术亮点
 * 当同时装备特定的主动法器和辅助法器时，触发额外共鸣效果
 * 展示组合检测、状态管理、动态效果计算等 Qt 高级技术
 */
class InstrumentResonance : public QObject
{
    Q_OBJECT
public:
    explicit InstrumentResonance(QObject *parent = nullptr);

    struct ResonanceInfo {
        QString name;           // 共鸣名称
        QString description;    // 效果描述
        QString icon;           // 图标/特效标识

        // 触发条件（必须同时满足）
        Instrument::Type requiredActive;   // 需要的主动法器
        QList<Instrument::Type> requiredAuxiliary; // 需要的辅助法器（至少一个）
        bool requireAllAuxiliary; // 是否需要同时装备所有辅助法器

        // 效果倍率
        double clickMultiplier;     // 点击收益倍率
        double autoIncomeBonus;     // 额外自动收益/秒
        double critRateBonus;       // 额外暴击率
        double critMultiplierBonus; // 额外暴击倍数加成
        double efficiencyBonus;     // 额外效率加成
        int duration;               // 效果持续时间（秒，0=永久）
    };

    // 更新当前装备状态，重新计算共鸣
    void updateResonance(const Instrument& activeInstrument,
                         const QList<Instrument>& auxiliaryInstruments);

    // 获取当前激活的共鸣列表
    QList<ResonanceInfo> activeResonances() const;

    // 计算总效果加成
    double totalClickMultiplier() const;
    double totalAutoIncomeBonus() const;
    double totalCritRateBonus() const;
    double totalCritMultiplierBonus() const;
    double totalEfficiencyBonus() const;

    // 获取共鸣效果的文本描述（用于 HUD 显示）
    QString getActiveResonanceText() const;

signals:
    void resonanceActivated(const QString& name, const QString& description);
    void resonanceDeactivated(const QString& name);

private:
    QList<ResonanceInfo> m_resonanceTable;
    QList<ResonanceInfo> m_activeResonances;

    void initializeResonanceTable();
    bool checkResonanceTriggered(const ResonanceInfo& resonance,
                                  const Instrument& active,
                                  const QList<Instrument>& auxiliaries) const;
};

#endif // INSTRUMENTRESONANCE_H
