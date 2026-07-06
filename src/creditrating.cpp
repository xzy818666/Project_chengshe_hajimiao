#include "creditrating.h"

CreditRating::CreditRating(QObject *parent) : QObject(parent),
    m_score(500), m_grade(B)
{
    updateGrade();
}

CreditRating::Grade CreditRating::grade() const { return m_grade; }

QString CreditRating::gradeText() const
{
    switch (m_grade) {
    case S: return "S";
    case A: return "A";
    case B: return "B";
    case C: return "C";
    case D: return "D";
    case F: return "F";
    default: return "?";
    }
}

int CreditRating::gradeScore() const { return m_score; }

CreditRating::Grade CreditRating::scoreToGrade(int score) const
{
    if (score >= 900) return S;
    if (score >= 750) return A;
    if (score >= 600) return B;
    if (score >= 450) return C;
    if (score >= 300) return D;
    return F;
}

void CreditRating::updateGrade()
{
    Grade newGrade = scoreToGrade(m_score);
    if (newGrade != m_grade) {
        m_grade = newGrade;
        emit gradeChanged(m_grade, gradeText());
    }
}

void CreditRating::addScore(int points)
{
    m_score = qMin(1000, m_score + points);
    emit scoreChanged(m_score);
    updateGrade();
}

void CreditRating::subScore(int points)
{
    m_score = qMax(0, m_score - points);
    emit scoreChanged(m_score);
    updateGrade();
}

void CreditRating::setScore(int score)
{
    m_score = qMax(0, qMin(1000, score));
    emit scoreChanged(m_score);
    updateGrade();
}

double CreditRating::loanRateMultiplier() const
{
    switch (m_grade) {
    case S: return 0.5;  // 利率减半
    case A: return 0.7;
    case B: return 1.0;  // 标准利率
    case C: return 1.3;
    case D: return 1.8;
    case F: return 3.0;  // 三倍利率
    default: return 1.0;
    }
}

double CreditRating::maxLeverageMultiplier() const
{
    switch (m_grade) {
    case S: return 1.5;  // 最高 1.5× 额外杠杆
    case A: return 1.2;
    case B: return 1.0;  // 标准杠杆
    case C: return 0.8;
    case D: return 0.5;
    case F: return 0.0;  // 禁止杠杆
    default: return 1.0;
    }
}

double CreditRating::creditLimitMultiplier() const
{
    switch (m_grade) {
    case S: return 2.0;  // 额度翻倍
    case A: return 1.5;
    case B: return 1.0;  // 标准额度
    case C: return 0.7;
    case D: return 0.4;
    case F: return 0.1;  // 几乎无法借款
    default: return 1.0;
    }
}

double CreditRating::transactionFeeDiscount() const
{
    switch (m_grade) {
    case S: return 0.0;  // 免交易费
    case A: return 0.002;
    case B: return 0.005;
    case C: return 0.01;
    case D: return 0.02;
    case F: return 0.05;  // 高额交易费
    default: return 0.005;
    }
}

void CreditRating::recordRepayment(bool onTime)
{
    if (onTime) {
        addScore(20); // 按时还款 +20
    } else {
        subScore(50); // 逾期 -50
    }
}

void CreditRating::recordLeverageLiquidation()
{
    subScore(100); // 爆仓 -100
}

void CreditRating::recordDailyActivity()
{
    addScore(2); // 每日活跃 +2
}

void CreditRating::recordYezhangImpact(int yezhang)
{
    if (yezhang > 500) {
        subScore(yezhang / 50); // 高业障扣分
    }
}
