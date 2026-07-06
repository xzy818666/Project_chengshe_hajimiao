#include "tutorialmanager.h"
#include <QDialog>

TutorialManager* TutorialManager::s_instance = nullptr;

TutorialManager* TutorialManager::instance()
{
    if (!s_instance) {
        s_instance = new TutorialManager();
    }
    return s_instance;
}

TutorialManager::TutorialManager(QObject *parent)
    : QObject(parent)
    , m_currentStep(0)
    , m_active(false)
    , m_completed(false)
{
    buildSteps();
}

void TutorialManager::buildSteps()
{
    // === 功德堂阶段 ===
    m_steps.append({0,
        "道友留步！贫道<b>玄机子</b>，赛博功德银行导航员。<br><br>"
        "记住：<b style='color:#FF8C00;'>打工敲木鱼是没有前途的。</b>",
        "玄机子", "happy", "", "none", "", false, true, 3000});

    m_steps.append({1,
        "这是你的主基地——<b>功德堂</b>。<br><br>"
        "看到云海中的法器了吗？<br>"
        "<b>点击它！</b>",
        "玄机子", "normal", "fishClickArea", "click_cloud", "", true, false, 0});

    m_steps.append({2,
        "每敲一次，功德+1。<br><br>"
        "但你注意到通胀率了吗？<br>"
        "<b style='color:#FF6B6B;'>每天默默吃掉你的功德。</b>",
        "玄机子", "worried", "", "none", "", false, true, 2500});

    m_steps.append({3,
        "只靠点击，永无出头之日。<br><br>"
        "先去<b>善财司</b>看看。<br>"
        "<b>点击右上角的建筑！</b>",
        "玄机子", "normal", "pavilionBankBtn", "click_area", "", true, false, 0});

    // === 善财司阶段（8步） ===
    m_steps.append({4,
        "欢迎来到<b>善财司</b>。<br><br>"
        "这里是功德的存储与借贷中枢。",
        "玄机子", "happy", "", "dialog_open", "BankDialog", false, false, 0});

    m_steps.append({5,
        "左边是<b>活期与定期</b>。<br><br>"
        "• 活期：年化5%，灵活存取<br>"
        "• 定期：7天6% / 30天8% / 90天12%<br><br>"
        "听着不错？但通胀1%/天……<br>"
        "<b style='color:#FF6B6B;'>存一年购买力缩水95%。</b>",
        "玄机子", "worried", "", "none", "BankDialog", false, false, 0});

    m_steps.append({6,
        "中间是<b>功德借贷</b>。<br><br>"
        "• 信用等级 0-1000 分<br>"
        "• 可借额度 = 信用分 × 2<br>"
        "• 日利率 1%，信用越高利率越低<br><br>"
        "借点钱应急可以，但……",
        "玄机子", "serious", "", "none", "BankDialog", false, false, 0});

    m_steps.append({7,
        "逾期惩罚很残酷：<br><br>"
        "• 按<b>秒复利</b>膨胀<br>"
        "• 超限后 10% 债务转<b style='color:#FF5252;'>业障</b><br><br>"
        "业障高了会引<b style='color:#FF5252;'>天劫</b>！",
        "玄机子", "worried", "", "none", "BankDialog", false, false, 0});

    m_steps.append({8,
        "信用等级怎么提升？<br><br>"
        "• 保持高功德余额<br>"
        "• 按时还款不逾期<br>"
        "• 多点击、多交易活跃账号<br><br>"
        "<b>信用是一切金融活动的基础。</b>",
        "玄机子", "serious", "", "none", "BankDialog", false, false, 0});

    m_steps.append({9,
        "善财司和幻缘所是<b>联动</b>的。<br><br>"
        "信用等级越高 → 借的钱越多 → 杠杆空间越大。<br><br>"
        "<b>一环扣一环，这才是金融的真谛。</b>",
        "玄机子", "happy", "", "none", "BankDialog", false, false, 0});

    m_steps.append({10,
        "记住这个原则：<br><br>"
        "<b style='color:#FF6B6B;'>存功德是慢性自杀，借功德是饮鸩止渴。</b><br><br>"
        "赚功德还得去幻缘所。<br>"
        "<b>请点击「返回」。</b>",
        "玄机子", "normal", "", "dialog_close", "BankDialog", false, false, 0});

    // === 法器阁阶段（7步） ===
    m_steps.append({11,
        "接下来去<b>法器阁</b>。<br><br>"
        "法器是放大器，不是发动机。<br>"
        "<b>点击左下角的建筑！</b>",
        "玄机子", "normal", "pavilionShopBtn", "click_area", "", true, false, 0});

    m_steps.append({12,
        "欢迎来到<b>法器阁</b>。<br><br>"
        "两大区域：主动法器和辅助法器。",
        "玄机子", "happy", "", "dialog_open", "ShopDialog", false, false, 0});

    m_steps.append({13,
        "<b>主动法器</b>（装备到云上）：<br><br>"
        "• 基础木鱼：0功德，+1/击<br>"
        "• 涡轮增压木鱼：2000功德，+5/击<br>"
        "• 量子佛珠：8000功德，+2/击，<b>10%暴击×10</b>",
        "玄机子", "normal", "", "none", "ShopDialog", false, false, 0});

    m_steps.append({14,
        "<b>辅助法器</b>（装备到莲台）：<br><br>"
        "• 电子烧香：500功德，+1/秒<br>"
        "• AI诵经机：15000功德，+10/秒，维护费1/秒<br><br>"
        "可同时装备多件，自动收益累加。",
        "玄机子", "normal", "", "none", "ShopDialog", false, false, 0});

    m_steps.append({15,
        "法器的核心秘密是<b>组合共鸣</b>！<br><br>"
        "特定搭配触发额外效果：<br>"
        "• 涡轮木鱼 + 电子烧香：点击+20%<br>"
        "• 量子佛珠 + AI诵经机：暴击+15%<br>"
        "• 三件套同时：全部收益+30%",
        "玄机子", "surprised", "", "none", "ShopDialog", false, false, 0});

    m_steps.append({16,
        "但记住核心原则：<br><br>"
        "1. <b>法器是放大器，不是发动机</b><br>"
        "2. <b>优先投资幻缘所赚本金</b><br>"
        "3. <b>辅助法器有维护费</b><br><br>"
        "没本金，再好的法器也白搭。<br>"
        "<b>请点击「返回」。</b>",
        "玄机子", "serious", "", "dialog_close", "ShopDialog", false, false, 0});

    // === 幻缘所阶段（12步） ===
    m_steps.append({17,
        "现在去<b>幻缘所</b>——<br><br>"
        "这才是功德暴涨的<b style='color:#FFD700;'>真传</b>！<br>"
        "<b>点击左上角的建筑！</b>",
        "玄机子", "happy", "pavilionExchangeBtn", "click_area", "", true, false, 0});

    m_steps.append({18,
        "欢迎来到<b>幻缘所</b>——赛博功德交易所！<br><br>"
        "这里买卖四种资产，低买高卖赚差价。",
        "玄机子", "happy", "", "dialog_open", "ExchangeDialog", false, false, 0});

    m_steps.append({19,
        "看左边——<b>资产列表</b>。<br><br>"
        "• <b style='color:#4FC3F7;'>众生法缘</b>：中风险，跟大盘波动<br>"
        "• <b style='color:#81C784;'>善行福缘</b>：低风险，价格恒定1.0<br>"
        "• <b style='color:#CE93D8;'>因果私缘</b>：混合基金<br>"
        "• <b style='color:#FF5252;'>轮回孽缘</b>：高风险，波动×3",
        "玄机子", "normal", "", "none", "ExchangeDialog", false, false, 0});

    m_steps.append({20,
        "右边是<b>交易面板</b>。<br><br>"
        "• K线图显示最近100个价格点<br>"
        "• 输入数量和杠杆倍数<br>"
        "• 点击「买入」或「卖出」<br><br>"
        "支持1×到3×杠杆！",
        "玄机子", "surprised", "", "none", "ExchangeDialog", false, false, 0});

    m_steps.append({21,
        "<b style='color:#FFD700;'>套利</b>——最稳的赚钱方式！<br><br>"
        "因果私缘理论价 = 0.6×众生法缘 + 0.4×善行福缘<br><br>"
        "市场价格偏离这个公式？<br>"
        "<b style='color:#81C784;'>白捡的钱！</b>",
        "玄机子", "happy", "", "none", "ExchangeDialog", false, false, 0});

    m_steps.append({22,
        "系统会自动检测套利机会。<br><br>"
        "看到底部「智能推荐」了吗？<br>"
        "点击那里查看实时套利提示。<br><br>"
        "<b>29种校园主题市场事件</b>——每次事件都是机会！",
        "玄机子", "normal", "", "none", "ExchangeDialog", false, false, 0});

    m_steps.append({23,
        "<b style='color:#FFD700;'>杠杆</b>——双刃剑。<br><br>"
        "• 最大1×到3×（难度决定）<br>"
        "• 借银行的钱以小博大<br>"
        "• <b style='color:#FF5252;'>维持率&lt;30% 强制平仓！</b><br><br>"
        "爆仓后亏损优先扣下世储备。",
        "玄机子", "worried", "", "none", "ExchangeDialog", false, false, 0});

    m_steps.append({24,
        "现在看另一个面板。<br><br>"
        "<b>点击顶部的「资产配置」标签！</b>",
        "玄机子", "normal", "", "none", "ExchangeDialog", false, false, 0});

    m_steps.append({25,
        "这里是<b>资产配置</b>面板。<br><br>"
        "• 饼图显示当前持仓占比<br>"
        "• <b>CAL风险-收益图</b>：蓝色散点是各资产，红点是你<br>"
        "• 资本市场线帮你找到最优组合",
        "玄机子", "happy", "", "none", "ExchangeDialog", false, false, 0});

    m_steps.append({26,
        "还有<b>自动定投</b>功能。<br><br>"
        "设置间隔和金额，系统自动购买。<br>"
        "摊平成本，不用盯盘。<br><br>"
        "但记住：<b>再好的算法也不能消除风险。</b>",
        "玄机子", "serious", "", "none", "ExchangeDialog", false, false, 0});

    m_steps.append({27,
        "幻缘所的核心策略：<br><br>"
        "1. <b>套利优先</b>——风险最低<br>"
        "2. <b>分散持仓</b>——不要All in<br>"
        "3. <b>控制杠杆</b>——留足保证金<br>"
        "4. <b>善用定投</b>——摊平成本<br><br>"
        "<b>请点击「返回」。</b>",
        "玄机子", "serious", "", "dialog_close", "ExchangeDialog", false, false, 0});

    // === 成就阶段（5步） ===
    m_steps.append({28,
        "最后去看看<b>成就榜</b>。<br><br>"
        "<b>点击右下角的建筑！</b>",
        "玄机子", "normal", "pavilionAchievementBtn", "click_area", "", true, false, 0});

    m_steps.append({29,
        "欢迎来到<b>成就榜</b>。<br><br>"
        "记录你在轮回中的足迹。",
        "玄机子", "happy", "", "dialog_open", "AchievementDialog", false, false, 0});

    m_steps.append({30,
        "8个成就等你解锁：<br><br>"
        "• 初入佛门、腱鞘炎警告<br>"
        "• 华尔街之狼、负债累累<br>"
        "• 功德无量、天台见<br>"
        "• 资产配置大师、杠杆赌徒",
        "玄机子", "happy", "", "none", "AchievementDialog", false, false, 0});

    m_steps.append({31,
        "每次轮回结算，<b>业障</b>和<b>下世储备</b>会传承。<br><br>"
        "业障多了 → <b style='color:#FF5252;'>下一世困难模式</b><br>"
        "天劫降临，资产腰斩。<br><br>"
        "• 忏悔消业障：1功德 = 1业障<br>"
        "• 下世储备：轮回孽缘亏损优先扣除",
        "玄机子", "worried", "", "none", "AchievementDialog", false, false, 0});

    m_steps.append({32,
        "财富密码总结：<br><br>"
        "<b>善财司借贷 + 幻缘所杠杆 + 法器放大</b><br><br>"
        "但再好的装备也不能替代风控。<br>"
        "<b>请点击「返回」完成教程。</b>",
        "玄机子", "happy", "", "dialog_close", "AchievementDialog", false, false, 0});

    // === 完成 ===
    m_steps.append({33,
        "🎉 <b style='color:#FFD700;'>教程完成！</b> 🎉<br><br>"
        "你已掌握全部要诀。<br><br>"
        "去吧，在轮回中赚取第一桶金！<br><br>"
        "<span style='color:#888;'>(亏了别说是我教的。——玄机子)</span>",
        "玄机子", "happy", "", "none", "", false, true, 4000});
}

const TutorialManager::Step* TutorialManager::currentStep() const
{
    if (m_currentStep >= 0 && m_currentStep < m_steps.size()) {
        return &m_steps[m_currentStep];
    }
    return nullptr;
}

void TutorialManager::startTutorial()
{
    if (m_active) return;
    m_active = true;
    m_completed = false;
    m_currentStep = 0;
    emit tutorialStarted();
    emit stepChanged(m_currentStep);
}

void TutorialManager::endTutorial()
{
    if (!m_active) return;
    m_active = false;
    m_completed = true;
    markCompleted();
    emit tutorialFinished();
}

void TutorialManager::nextStep()
{
    if (!m_active) return;
    m_currentStep++;
    if (m_currentStep >= m_steps.size()) {
        endTutorial();
        return;
    }
    emit stepChanged(m_currentStep);
}

void TutorialManager::skipTutorial()
{
    if (!m_active) return;
    m_active = false;
    markCompleted();
    emit tutorialSkipped();
}

void TutorialManager::notifyDialogOpened(QDialog* dialog, const QString& dialogName)
{
    if (!m_active || !dialog) return;
    const Step* step = currentStep();
    if (!step) return;

    if (step->waitFor == "dialog_open" && step->targetDialog == dialogName) {
        emit dialogOpened(dialog);
        QTimer::singleShot(500, this, [this]() { nextStep(); });
    }
}

void TutorialManager::notifyDialogClosed(const QString& dialogName)
{
    if (!m_active) return;

    emit dialogClosed();

    const Step* step = currentStep();
    if (!step) return;

    if (step->waitFor == "dialog_close" && step->targetDialog == dialogName) {
        nextStep();
    }
}

void TutorialManager::notifyAreaClicked(const QString& areaName)
{
    if (!m_active) return;
    const Step* step = currentStep();
    if (!step) return;

    if (step->waitFor == "click_area" && step->targetWidget == areaName) {
        nextStep();
    }
    else if (step->waitFor == "click_cloud" && areaName == "fishClickArea") {
        nextStep();
    }
}

bool TutorialManager::hasCompletedBefore() const
{
    return false;
}

void TutorialManager::markCompleted()
{
}
