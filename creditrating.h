#ifndef CREDITRATING_H
#define CREDITRATING_H

#include <QObject>
#include <QString>

/**
 * 银行信用等级动态评级系统
 * 信用等级影响借贷利率、最大杠杆、可借额度
 * 通过活跃度、还款记录、资产净值动态调整
 */
class CreditRating : public QObject
{
    Q_OBJECT
public:
    explicit CreditRating(QObject *parent = nullptr);

    enum Grade { S = 0, A = 1, B = 2, C = 3, D = 4, F = 5 };

    Grade grade() const;
    QString gradeText() const;
    int gradeScore() const; // 0-1000

    // 等级影响
    double loanRateMultiplier() const;  // 借贷利率倍率
    double maxLeverageMultiplier() const; // 最大杠杆倍率
    double creditLimitMultiplier() const; // 可借额度倍率
    double transactionFeeDiscount() const; // 交易费折扣

    // 信用分调整
    void addScore(int points);   // 正面行为加分
    void subScore(int points);   // 负面行为减分
    void setScore(int score);    // 直接设置

    // 行为记录
    void recordRepayment(bool onTime);      // 记录还款
    void recordLeverageLiquidation();       // 记录爆仓
    void recordDailyActivity();             // 记录活跃
    void recordYezhangImpact(int yezhang);  // 业障影响

signals:
    void gradeChanged(Grade newGrade, QString text);
    void scoreChanged(int score);

private:
    int m_score; // 0-1000
    Grade m_grade;

    void updateGrade();
    Grade scoreToGrade(int score) const;
};

#endif // CREDITRATING_H
