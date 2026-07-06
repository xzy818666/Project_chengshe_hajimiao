#include "tutorialdialog.h"
#include <QGraphicsDropShadowEffect>

TutorialDialog::TutorialDialog(QWidget *parent)
    : QDialog(parent, Qt::Dialog | Qt::FramelessWindowHint)
    , m_currentStep(0)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(720, 520);
    setupUI();
}

void TutorialDialog::setupUI()
{
    // 主背景
    QWidget *bg = new QWidget(this);
    bg->setGeometry(0, 0, 720, 520);
    bg->setStyleSheet(
        "QWidget {"
        "  background-color: rgba(20, 15, 10, 0.96);"
        "  border: 3px solid rgba(200, 160, 80, 0.8);"
        "  border-radius: 20px;"
        "}"
    );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 20, 30, 20);
    mainLayout->setSpacing(12);

    // 顶部：角色名 + 进度
    QHBoxLayout *headerLayout = new QHBoxLayout();

    QLabel *roleLabel = new QLabel("☯ 赛博引路人·玄机子", this);
    roleLabel->setStyleSheet(
        "font-size: 18px; font-weight: bold; color: #FFD700;"
        "font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
    );
    headerLayout->addWidget(roleLabel);

    headerLayout->addStretch();

    m_stepIndicator = new QLabel("1 / 7", this);
    m_stepIndicator->setStyleSheet(
        "font-size: 13px; color: #B0B0B0;"
        "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
    );
    headerLayout->addWidget(m_stepIndicator);

    mainLayout->addLayout(headerLayout);

    // 进度条
    m_progressBar = new QProgressBar(this);
    m_progressBar->setMaximum(TOTAL_STEPS);
    m_progressBar->setValue(1);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedHeight(6);
    m_progressBar->setStyleSheet(
        "QProgressBar {"
        "  background-color: rgba(60, 50, 40, 0.8);"
        "  border-radius: 3px;"
        "}"
        "QProgressBar::chunk {"
        "  background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 #5D4037, stop:1 #FFD700);"
        "  border-radius: 3px;"
        "}"
    );
    mainLayout->addWidget(m_progressBar);

    // 内容区域
    m_stack = new QStackedWidget(this);
    m_stack->setFixedHeight(360);

    // === 步骤 0：引子 ===
    createStep(0,
        "初入轮回",
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "道友留步！贫道<b style='color:#FFD700;'>玄机子</b>，乃赛博功德银行新晋导航员。"
        "别看我一身道袍，我可是持证上岗的——喏，这是我的区块链道士证（掏出发光平板）。</p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "既然入了这轮回，贫道就带你走一遭。记住一句话：<br>"
        "<b style='color:#FF8C00; font-size:17px;'>在这赛博仙界，打工敲木鱼是没有前途的。</b></p>"
        "<p style='font-size:14px; line-height:1.6; color:#B0B0B0; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "（喝一口枸杞泡的可乐）来，先熟悉一下地形。</p>",
        "提示：点击「下一步」开始游览",
        "🧙"
    );

    // === 步骤 1：功德堂 ===
    createStep(1,
        "功德堂 · 牛马的起跑线",
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "这是功德堂，你的主基地。看到中间那朵云了吗？<b>点它！</b></p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "每敲一次木鱼，功德+1。听起来很美对吧？但你看这通胀率——每天默默吃掉你的功德。"
        "敲一个月，可能刚跑赢通胀。</p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "<b style='color:#FF6B6B;'>这就是现代牛马的宿命：</b>你拼命敲，通胀默默偷。"
        "只靠打工，永无出头之日。（摇头叹气）</p>",
        "提示：效率系数越高，每次点击收益越多",
        "☁️"
        );

    // === 步骤 2：善财司 ===
    createStep(2,
        "善财司 · 存钱的陷阱",
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "这里是善财司，存贷业务。活期年化5%，定期最高12%。听着不错？</p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "来，贫道给你算笔账：假设通胀率1%/天，一年下来你的购买力缩水了......"
        "（掏出计算器猛按）算了，你直接看结论吧：</p>"
        "<p style='font-size:16px; line-height:1.8; color:#FF6B6B; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "<b>存定期？那是在慢性自杀。</b></p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "你的功德在保险箱里睡大觉，通胀在门外偷功德。跑不赢，永远跑不赢。"
        "当然，借点钱应急还是可以的——<b style='color:#81C784;'>信用等级越高，利率越低。</b></p>",
        "提示：信用等级S可享利率减半，F级利率三倍",
        "🏦"
    );

    // === 步骤 3：法器阁 ===
    createStep(3,
        "法器阁 · 放大器不是发动机",
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "法器阁！看到那些酷炫装备了吗？涡轮增压木鱼、量子佛珠、全视之眼......"
        "（眼冒金光）</p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "但看看价格——量子佛珠<b>8000</b>功德，AI诵经机<b>15000</b>。"
        "你靠敲木鱼一天能攒多少？200？300？要攒到猴年马月？</p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "更关键的是，<b style='color:#E1BEE7;'>好法器可以组合共鸣！</b>"
        "比如涡轮增压+电子烧香=烈火焚心，点击收益×1.5。"
        "但前提是......你得买得起。（摊手）</p>"
        "<p style='font-size:14px; line-height:1.6; color:#B0B0B0; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "所以法器是放大器，不是发动机。没有本金，再好的法器也是摆设。</p>",
        "提示：拂尘+如意+罗盘同时装备可触发「三清合一」共鸣",
        "⚔️"
    );

    // === 步骤 4：幻缘所（核心）===
    createStep(4,
        "幻缘所 · 真传所在（上）",
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "（压低声音，环顾四周）道友，接下来是贫道要传授的<b style='color:#FFD700;'>真传</b>了。</p>"
        "<p style='font-size:16px; line-height:1.8; color:#FF8C00; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "<b>幻缘所——这才是功德暴涨的唯一出路。</b></p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "这里交易四种资产：<br>"
        "• <b style='color:#81C784;'>善行福缘</b>：低风险，固定收益，适合躺平<br>"
        "• <b style='color:#4FC3F7;'>众生法缘</b>：中风险，跟大盘走<br>"
        "• <b style='color:#CE93D8;'>因果私缘</b>：混合基金，价格有规律<br>"
        "• <b style='color:#FF5252;'>轮回孽缘</b>：高风险高回报，自带杠杆！</p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "看到那个K线图了吗？红绿交错，那是贫道的K线图，"
        "也是无数道友的......血泪史。（意味深长）</p>",
        "提示：CAL红线显示最优风险收益组合",
        "📈"
    );

    // === 步骤 5：幻缘所（核心下）===
    createStep(5,
        "幻缘所 · 真传所在（下）",
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "<b style='color:#FFD700;'>套利：</b>简单说，因果私缘的理论价格 = 0.6×众生法缘 + 0.4×善行福缘。"
        "如果实际价格偏离了这个公式——<b style='color:#81C784;'>那就是白捡的钱！</b>低买高卖，无风险套利。</p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "<b style='color:#FFD700;'>杠杆：</b>右边那个下拉框，1×到3×。借银行的钱去投资，以小博大。"
        "但记住——<b style='color:#FF5252;'>杠杆是一把双刃剑。</b>"
        "爆仓了债务转业障，业障高了引天劫。维持率跌破30%直接强制平仓，没得商量。</p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "善财司的借贷和幻缘所的杠杆是<b>联动</b>的——信用等级越高，"
        "能借的钱越多，杠杆空间越大。<b style='color:#FFD700;'>一环扣一环，这才是金融的真谛。</b></p>",
        "提示：轮回孽缘自带2×杠杆，再选3×就是6×总杠杆！",
        "⚡"
    );

    // === 步骤 6：成就与轮回 ===
    createStep(6,
        "成就榜 · 轮回的传承",
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "最后是成就榜。这不仅仅是虚名——每一次轮回结算，"
        "你的<b>业障</b>和<b>下世储备</b>会传承到下一轮。</p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "如果这一世积累了大量业障......（脸色凝重）"
        "<b style='color:#FF5252;'>下一世开局就是困难模式。</b>"
        "天劫降临，资产腰斩，那画面太美我不敢看。</p>"
        "<p style='font-size:16px; line-height:1.8; color:#FFD700; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "<b>所以记住：</b></p>"
        "<p style='font-size:15px; line-height:1.8; color:#F5F5DC; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "<b style='color:#81C784;'>控制业障，善用杠杆，在幻缘所里搏出一片天。</b><br>"
        "打工？那只是你启动资金的第一桶金。</p>"
        "<p style='font-size:14px; line-height:1.6; color:#B0B0B0; font-family:STKaiti,KaiTi,SimSun,serif;'>"
        "（突然凑近）对了，如果亏了，别说是我教的。（眨眼）</p>",
        "提示：完成特定成就可以获得额外轮回奖励",
        "🏆"
    );

    mainLayout->addWidget(m_stack);

    // 底部按钮
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    m_prevBtn = new QPushButton("← 上一步", this);
    m_prevBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(93, 64, 55, 0.88);"
        "  color: #FFD700;"
        "  border: 2px solid rgba(200, 160, 80, 0.7);"
        "  border-radius: 8px;"
        "  padding: 8px 20px;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(200, 160, 80, 0.85);"
        "  color: #3E2723;"
        "}"
        "QPushButton:disabled {"
        "  background-color: rgba(60, 50, 40, 0.5);"
        "  color: #666666;"
        "  border: 2px solid rgba(100, 80, 50, 0.3);"
        "}"
    );
    m_prevBtn->setCursor(Qt::PointingHandCursor);

    m_skipBtn = new QPushButton("跳过教程", this);
    m_skipBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent;"
        "  color: #888888;"
        "  border: 1px solid rgba(100, 100, 100, 0.4);"
        "  border-radius: 6px;"
        "  padding: 6px 16px;"
        "  font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "  color: #FFD700;"
        "  border: 1px solid rgba(255, 215, 0, 0.5);"
        "}"
    );
    m_skipBtn->setCursor(Qt::PointingHandCursor);

    m_nextBtn = new QPushButton("下一步 →", this);
    m_nextBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #5D4037;"
        "  color: #FFD700;"
        "  border: 2px solid #FFD700;"
        "  border-radius: 8px;"
        "  padding: 8px 24px;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #FFD700;"
        "  color: #3E2723;"
        "}"
    );
    m_nextBtn->setCursor(Qt::PointingHandCursor);

    btnLayout->addWidget(m_prevBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_skipBtn);
    btnLayout->addWidget(m_nextBtn);
    mainLayout->addLayout(btnLayout);

    // 连接信号
    connect(m_nextBtn, &QPushButton::clicked, this, &TutorialDialog::nextStep);
    connect(m_prevBtn, &QPushButton::clicked, this, &TutorialDialog::prevStep);
    connect(m_skipBtn, &QPushButton::clicked, this, &TutorialDialog::skipTutorial);

    m_prevBtn->setEnabled(false);
    updateStepDisplay();
}

void TutorialDialog::createStep(int index, const QString& title,
                                 const QString& content, const QString& tip,
                                 const QString& icon)
{
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 5, 10, 5);

    // 标题
    QLabel *titleLabel = new QLabel(QString("%1 %2").arg(icon).arg(title));
    titleLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #FFD700;"
        "font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
    );
    titleLabel->setAlignment(Qt::AlignLeft);
    layout->addWidget(titleLabel);

    // 分隔线
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: rgba(200, 160, 80, 0.4);");
    line->setFixedHeight(2);
    layout->addWidget(line);

    // 内容（支持HTML）
    QLabel *contentLabel = new QLabel();
    contentLabel->setText(content);
    contentLabel->setWordWrap(true);
    contentLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    contentLabel->setTextFormat(Qt::RichText);
    contentLabel->setOpenExternalLinks(false);
    layout->addWidget(contentLabel, 1);

    // 底部提示
    QLabel *tipLabel = new QLabel(tip);
    tipLabel->setStyleSheet(
        "font-size: 12px; color: #888888;"
        "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
    );
    tipLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(tipLabel);

    m_stack->addWidget(page);
}

void TutorialDialog::nextStep()
{
    if (m_currentStep < TOTAL_STEPS - 1) {
        m_currentStep++;
        m_stack->setCurrentIndex(m_currentStep);
        updateStepDisplay();
    } else {
        accept(); // 完成教程
    }
}

void TutorialDialog::prevStep()
{
    if (m_currentStep > 0) {
        m_currentStep--;
        m_stack->setCurrentIndex(m_currentStep);
        updateStepDisplay();
    }
}

void TutorialDialog::skipTutorial()
{
    reject(); // 跳过
}

void TutorialDialog::updateStepDisplay()
{
    m_stepIndicator->setText(QString("%1 / %2").arg(m_currentStep + 1).arg(TOTAL_STEPS));
    m_progressBar->setValue(m_currentStep + 1);
    m_prevBtn->setEnabled(m_currentStep > 0);

    if (m_currentStep == TOTAL_STEPS - 1) {
        m_nextBtn->setText("踏入轮回 ✓");
    } else {
        m_nextBtn->setText("下一步 →");
    }
}
