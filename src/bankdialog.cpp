#include "bankdialog.h"
#include "ui_bankdialog.h"
#include "tutorialmanager.h"
#include <QMessageBox>
#include <QFrame>
#include <QTableWidgetItem>
#include <QLabel>
#include <QTimer>
#include <QRandomGenerator>
#include <QPushButton>

const double BankDialog::FIXED_RATE_7DAY = 0.06;
const double BankDialog::FIXED_RATE_30DAY = 0.08;
const double BankDialog::FIXED_RATE_90DAY = 0.12;

BankDialog::BankDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BankDialog)
    , m_wallet(nullptr)
    , m_cloudFrame(nullptr)
    , m_beastText(nullptr)
    , m_talkTimer(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("善财司");

    // 适配主界面实际尺寸（默认回退 960×540）
    QSize sz = parent ? parent->size() : QSize(960, 540);
    resize(sz.width(), sz.height());
    setMinimumSize(800, 450);

    // 背景图按当前窗口尺寸缩放
    QPixmap bg(":/images/merit_bank.png");
    QPalette palette;
    palette.setBrush(QPalette::Window, bg.scaled(sz, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setPalette(palette);
    setAutoFillBackground(true);

    // 让标签页占据右半部分（spacer 与 tabWidget 各占 1:1）
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
    ui->horizontalLayout->setStretch(0, 1);
    ui->horizontalLayout->setStretch(1, 1);

    // 标签页半透明，透出背景图
    ui->tabWidget->setStyleSheet(
        "QTabWidget::pane { "
        "  background-color: rgba(30, 40, 60, 0.40); "
        "  border: 1px solid rgba(255, 215, 100, 0.25); "
        "} "
        "QTabBar::tab { "
        "  background-color: rgba(45, 45, 45, 0.50); "
        "  color: #E0E0E0; "
        "  padding: 8px 16px; "
        "} "
        "QTabBar::tab:selected { "
        "  background-color: rgba(30, 40, 60, 0.65); "
        "  color: #FFD700; "
        "} "
        "QTabBar::tab:hover { "
        "  background-color: rgba(60, 60, 60, 0.60); "
        "}"
    );
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        ui->tabWidget->widget(i)->setAutoFillBackground(false);
    }

    connect(ui->depositBtn, &QPushButton::clicked, this, &BankDialog::onDepositSavings);
    connect(ui->withdrawBtn, &QPushButton::clicked, this, &BankDialog::onWithdrawSavings);
    connect(ui->buyFD7Btn, &QPushButton::clicked, [this]() { onBuyFixedDeposit(7, FIXED_RATE_7DAY); });
    connect(ui->buyFD30Btn, &QPushButton::clicked, [this]() { onBuyFixedDeposit(30, FIXED_RATE_30DAY); });
    connect(ui->buyFD90Btn, &QPushButton::clicked, [this]() { onBuyFixedDeposit(90, FIXED_RATE_90DAY); });
    connect(ui->withdrawFDBtn, &QPushButton::clicked, this, &BankDialog::onWithdrawFixedDeposit);
    connect(ui->borrowBtn, &QPushButton::clicked, this, &BankDialog::onBorrow);
    connect(ui->repayBtn, &QPushButton::clicked, this, &BankDialog::onRepay);
    connect(ui->confessBtn, &QPushButton::clicked, this, &BankDialog::onConfess);
    connect(ui->addMarginBtn, &QPushButton::clicked, this, &BankDialog::onAddMargin);
    connect(ui->closeAllLeverageBtn, &QPushButton::clicked, this, &BankDialog::onCloseAllLeverage);

    // 教程支持：点击"功德借贷"标签和"借贷"按钮时通知教程管理器
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        if (index == 1 && TutorialManager::instance()->isActive()) {
            TutorialManager::instance()->notifyAreaClicked("bankLoanTab");
        }
    });
    connect(ui->borrowBtn, &QPushButton::clicked, this, []() {
        if (TutorialManager::instance()->isActive()) {
            TutorialManager::instance()->notifyAreaClicked("bankBorrowBtn");
        }
    });

    setupBeastTalk();

    QPushButton* backBtn = new QPushButton("⟵ 返回", this);
    backBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(93, 64, 55, 0.88);"
        "  color: #FFD700;"
        "  border: 2px solid rgba(255, 215, 0, 0.6);"
        "  border-radius: 8px;"
        "  padding: 6px 16px;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(255, 215, 0, 0.85);"
        "  color: #3E2723;"
        "}"
    );
    backBtn->setGeometry(width() - 110, 14, 90, 36);
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->raise();
    connect(backBtn, &QPushButton::clicked, this, &BankDialog::accept);
}

BankDialog::~BankDialog()
{
    delete ui;
}

void BankDialog::setWallet(Wallet* wallet)
{
    m_wallet = wallet;
    connect(m_wallet, &Wallet::meritChanged, this, &BankDialog::updateInfo);
    connect(m_wallet, &Wallet::debtChanged, this, &BankDialog::updateInfo);
    connect(m_wallet, &Wallet::yezhangChanged, this, &BankDialog::updateInfo);
    connect(m_wallet, &Wallet::leverageChanged, this, &BankDialog::updateLeverageTable);
    updateInfo();
    updateFixedDeposits();
    updateLeverageTable();
}

void BankDialog::setAssets(QList<Asset*> assets)
{
    m_assets = assets;
}

void BankDialog::onDepositSavings()
{
    if (!m_wallet) return;
    double amount = ui->savingsAmount->value();
    if (amount <= 0) {
        showBeastTalk("invalid", false);
        return;
    }
    if (m_wallet->merit() >= amount) {
        m_wallet->addSavings(amount);
        showBeastTalk("deposit_savings", true);
    } else {
        showBeastTalk("deposit_savings", false);
    }
    updateInfo();
}

void BankDialog::onWithdrawSavings()
{
    if (!m_wallet) return;
    double amount = ui->savingsAmount->value();
    bool ok = m_wallet->withdrawSavings(amount);
    showBeastTalk("withdraw_savings", ok);
    updateInfo();
}

void BankDialog::onBuyFixedDeposit(int days, double rate)
{
    if (!m_wallet) return;
    double amount = ui->fdAmount->value();
    QString id = m_wallet->addFixedDeposit(amount, days, rate);
    if (!id.isEmpty()) {
        showBeastTalk("buy_fixed", true);
        updateFixedDeposits();
        updateInfo();
    } else {
        showBeastTalk("buy_fixed", false);
    }
}

void BankDialog::onWithdrawFixedDeposit()
{
    if (!m_wallet) return;
    QListWidgetItem* item = ui->fdList->currentItem();
    if (item) {
        QString id = item->data(Qt::UserRole).toString();
        m_wallet->withdrawFixedDeposit(id);
        showBeastTalk("withdraw_fixed", true);
        updateFixedDeposits();
        updateInfo();
    } else {
        showBeastTalk("withdraw_fixed", false);
    }
}

void BankDialog::onBorrow()
{
    if (!m_wallet) return;
    double amount = ui->borrowAmount->value();
    bool ok = m_wallet->borrow(amount);
    showBeastTalk("borrow", ok);
    updateInfo();
}

void BankDialog::onRepay()
{
    if (!m_wallet) return;
    double amount = ui->repayAmount->value();
    bool ok = m_wallet->repay(amount);
    if (ok && m_wallet->creditRating()) {
        m_wallet->creditRating()->recordRepayment(true);
    }
    showBeastTalk("repay", ok);
    updateInfo();
}

void BankDialog::onConfess()
{
    if (!m_wallet) return;
    double amount = ui->confessAmount->value();
    if (amount > 0 && m_wallet->merit() >= amount) {
        m_wallet->reduceYezhang(amount);
        showBeastTalk("confess", true);
    } else {
        showBeastTalk("confess", false);
    }
    updateInfo();
}

void BankDialog::onAddMargin()
{
    if (!m_wallet) return;
    int row = ui->leverageTable->currentRow();
    if (row < 0) {
        showBeastTalk("add_margin", false);
        showToast("提示", "请先选中一条杠杆持仓", "info");
        return;
    }
    QString assetId = ui->leverageTable->item(row, 0)->data(Qt::UserRole).toString();
    double amount = ui->savingsAmount->value(); // 复用活期金额输入框
    if (amount <= 0) {
        showBeastTalk("add_margin", false);
        showToast("提示", "请输入有效的追加金额", "info");
        return;
    }
    if (m_wallet->addMarginToPosition(assetId, amount)) {
        showBeastTalk("add_margin", true);
        updateLeverageTable();
        updateInfo();
        showToast("追加成功", "保证金已追加", "success");
    } else {
        showBeastTalk("add_margin", false);
        showToast("追加失败", "功德不足或无该持仓", "error");
    }
}

void BankDialog::onCloseAllLeverage()
{
    if (!m_wallet) return;
    auto positions = m_wallet->leveragePositions();
    if (positions.isEmpty()) {
        showBeastTalk("close_leverage", false);
        showToast("提示", "当前没有杠杆持仓", "info");
        return;
    }

    int closed = 0;
    for (const auto& pos : positions) {
        Asset* asset = nullptr;
        for (Asset* a : m_assets) {
            if (a->id() == pos.assetId) {
                asset = a;
                break;
            }
        }
        if (asset) {
            m_wallet->closeLeveragePosition(pos.assetId, pos.shares, asset->price());
            closed++;
        }
    }
    showBeastTalk("close_leverage", closed > 0);
    updateLeverageTable();
    updateInfo();
    showToast("平仓完成", QString("已平仓 %1 条杠杆持仓").arg(closed), "success");
}

void BankDialog::updateInfo()
{
    if (!m_wallet) return;

    ui->savingsBalance->setText(QString("活期余额: %1 功德").arg(m_wallet->savingsBalance(), 0, 'f', 2));

    double debt = m_wallet->totalDebt();
    ui->debtBalance->setText(QString("债务总额: %1 功德").arg(debt, 0, 'f', 2));
    if (debt > m_wallet->maxBorrow()) {
        ui->debtBalance->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");
    } else if (debt > m_wallet->maxBorrow() * 0.5) {
        ui->debtBalance->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    } else {
        ui->debtBalance->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    int score = (int)m_wallet->creditScore();
    ui->creditScore->setText(QString("信用分: %1 / 1000").arg(score));
    if (score < 300) {
        ui->creditScore->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");
    } else if (score < 500) {
        ui->creditScore->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    } else {
        ui->creditScore->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    ui->maxBorrow->setText(QString("可借额度: %1 功德").arg(m_wallet->maxBorrow(), 0, 'f', 2));

    // 信用等级显示
    if (m_wallet->creditRating()) {
        QString grade = m_wallet->creditRating()->gradeText();
        double rateMult = m_wallet->creditRating()->loanRateMultiplier();
        double levMult = m_wallet->creditRating()->maxLeverageMultiplier();
        ui->creditScore->setText(
            QString("信用分: %1 / 1000  [等级: %2]  利率倍率: %3×  杠杆倍率: %4×")
                .arg(score).arg(grade).arg(rateMult, 0, 'f', 1).arg(levMult, 0, 'f', 1)
        );
    }

    double yz = m_wallet->yezhang();
    ui->yezhangValue->setText(QString("业障值: %1 / 2000").arg(yz, 0, 'f', 0));
    if (yz > 1000) {
        ui->yezhangValue->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");
    } else if (yz > 500) {
        ui->yezhangValue->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    } else {
        ui->yezhangValue->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    double efficiency = m_wallet->efficiency() * 100;
    ui->efficiencyValue->setText(QString("效率系数: %1%").arg(efficiency, 0, 'f', 1));
    if (efficiency < 60) {
        ui->efficiencyValue->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");
    } else if (efficiency < 80) {
        ui->efficiencyValue->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    } else {
        ui->efficiencyValue->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    ui->yezhangBar->setMaximum(2000);
    ui->yezhangBar->setValue((int)yz);
    // 进度条颜色随业障值变化
    if (yz > 1000) {
        ui->yezhangBar->setStyleSheet("QProgressBar::chunk { background-color: #C62828; }");
    } else if (yz > 500) {
        ui->yezhangBar->setStyleSheet("QProgressBar::chunk { background-color: #E65100; }");
    } else {
        ui->yezhangBar->setStyleSheet("QProgressBar::chunk { background-color: #2E7D32; }");
    }
}

void BankDialog::updateFixedDeposits()
{
    if (!m_wallet) return;

    ui->fdList->clear();
    for (const auto& fd : m_wallet->fixedDeposits()) {
        QString status = (QDateTime::currentDateTime() >= fd.dueDate) ? "已到期" : "存期中";
        QString itemText = QString("%1 - %2天期 - %3 (%4)").arg(fd.amount, 0, 'f', 2).arg(fd.dueDate.date().dayOfYear() - QDateTime::currentDateTime().date().dayOfYear()).arg(fd.rate * 100, 0, 'f', 1).arg(status);
        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, fd.id);
        ui->fdList->addItem(item);
    }
}

void BankDialog::updateLeverageTable()
{
    if (!m_wallet) return;

    double levDebt = m_wallet->leverageDebt();
    ui->leverageDebtLabel->setText(QString("杠杆负债: %1 功德").arg(levDebt, 0, 'f', 2));
    if (levDebt > 0) {
        ui->leverageDebtLabel->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");
    } else {
        ui->leverageDebtLabel->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    double borrowed = m_wallet->totalLeverageBorrowed();
    ui->totalBorrowedLabel->setText(QString("当前借款: %1 功德").arg(borrowed, 0, 'f', 2));
    if (borrowed > 0) {
        ui->totalBorrowedLabel->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    } else {
        ui->totalBorrowedLabel->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    }

    ui->leverageTable->setRowCount(0);
    int row = 0;
    for (const auto& pos : m_wallet->leveragePositions()) {
        ui->leverageTable->insertRow(row);

        // 查找资产名称和价格
        QString assetName = pos.assetId;
        double price = 0;
        for (Asset* a : m_assets) {
            if (a->id() == pos.assetId) {
                assetName = a->name();
                price = a->price();
                break;
            }
        }

        double marginRate = m_wallet->marginRate(pos.assetId, price);
        QString rateText = QString("%1%").arg(marginRate * 100, 0, 'f', 1);
        if (marginRate < 0.20) {
            rateText += " (危险)";
        } else if (marginRate < 0.35) {
            rateText += " (预警)";
        }

        QTableWidgetItem* nameItem = new QTableWidgetItem(assetName);
        nameItem->setData(Qt::UserRole, pos.assetId);
        ui->leverageTable->setItem(row, 0, nameItem);
        ui->leverageTable->setItem(row, 1, new QTableWidgetItem(QString::number(pos.shares, 'f', 4)));
        ui->leverageTable->setItem(row, 2, new QTableWidgetItem(QString::number(pos.principal, 'f', 2)));
        ui->leverageTable->setItem(row, 3, new QTableWidgetItem(QString::number(pos.borrowed, 'f', 2)));
        ui->leverageTable->setItem(row, 4, new QTableWidgetItem(rateText));
        row++;
    }
}

void BankDialog::setupBeastTalk()
{
    double sx = width() / 960.0;
    double sy = height() / 540.0;
    // 以原右上角 (520,150) 为基准，等比例缩至 2/3 后再稍微放大：240×96
    int cw = qRound(240 * sx);
    int ch = qRound(96 * sy);
    // 使用独立工具窗，彻底摆脱与 tabWidget 的 z-order 竞争
    m_cloudFrame = new QWidget(this, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);
    m_cloudFrame->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_cloudFrame->setAttribute(Qt::WA_ShowWithoutActivating);
    m_cloudFrame->setAttribute(Qt::WA_TranslucentBackground);
    m_cloudFrame->setFixedSize(cw, ch);
    m_cloudFrame->setAutoFillBackground(false);
    m_cloudFrame->hide();

    QLabel* bgLabel = new QLabel(m_cloudFrame);
    bgLabel->setGeometry(0, 0, cw, ch);
    QPixmap cloud(":/images/cloud_frame.png");
    if (!cloud.isNull()) {
        bgLabel->setPixmap(cloud.scaled(cw, ch, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
    bgLabel->setScaledContents(false);

    m_beastText = new QLabel(m_cloudFrame);
    m_beastText->setGeometry(0, 0, cw, ch);
    m_beastText->setAlignment(Qt::AlignCenter);
    m_beastText->setWordWrap(true);
    m_beastText->setStyleSheet(
        "QLabel {"
        "  color: #773b15;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "  background: transparent;"
        "}"
    );

    m_talkTimer = new QTimer(this);
    m_talkTimer->setSingleShot(true);
    connect(m_talkTimer, &QTimer::timeout, this, [this]() {
        if (m_cloudFrame) m_cloudFrame->hide();
    });
}

void BankDialog::showBeastTalk(const QString& actionType, bool success)
{
    if (!m_cloudFrame || !m_beastText) return;

    QStringList list;
    if (actionType == "deposit_savings") {
        list = success
            ? QStringList{"功德入库，利滚利来，善哉善哉。", "积少成多，聚沙成塔，此乃理财之根本。", "存下的不仅是功德，更是来世的底气。"}
            : QStringList{"施主，袋中功德不足，莫要强求。", "巧妇难为无米之炊，先去赚些功德吧。"};
    } else if (actionType == "withdraw_savings") {
        list = success
            ? QStringList{"取之于库，用之于道，量力而行。", "功德虽可取，莫忘留后路。", "细水长流，切忌涸泽而渔。"}
            : QStringList{"活期余额见底，施主请三思。", "库中空无一物，拿什么给你？"};
    } else if (actionType == "buy_fixed") {
        list = success
            ? QStringList{"以时间换空间，此乃大智慧。", "锁住功德，静待花开，稳如泰山。", "定期如闭关，到期即出关。"}
            : QStringList{"功德不足，闭关失败，先去化缘吧。", "囊中羞涩，何以定期？"};
    } else if (actionType == "withdraw_fixed") {
        list = success
            ? QStringList{"提前出关，功德折损，望施主三思。", "破了定力，少了利息，奈何奈何。", "因果未到，强行摘果，味道稍涩。"}
            : QStringList{"并无定期可取，施主莫要空欢喜。", "选中一条定期，方可提前支取。"};
    } else if (actionType == "borrow") {
        list = success
            ? QStringList{"借功德如借东风，用得好可助火势，用不好引火烧身。", "天行健，君子以自强不息；借功德，施主须量力而行。", "债务缠身，业障亦随之而来，慎之慎之。"}
            : QStringList{"信用额度已满，天道也不允你再借了。", "施主负债累累，先还清旧账再说吧。"};
    } else if (actionType == "repay") {
        list = success
            ? QStringList{"无债一身轻，功德圆满时。", "还一份债，清一分因果，善莫大焉。", "昔日借东风，今日还甘霖，此乃诚信之道。"}
            : QStringList{"功德不足以还债，巧妇难为无米之炊。", "口袋空空，如何还债？先去赚功德吧。"};
    } else if (actionType == "confess") {
        list = success
            ? QStringList{"放下屠刀，立地成佛。消一分业障，增一分清明。", "知错能改，善莫大焉。业障消散，功德自来。", "忏悔如洗涤，心净则佛土净。"}
            : QStringList{"功德不够，无法忏悔。心诚则灵，财到则清。", "想要消业障，也得有功德做燃料啊。"};
    } else if (actionType == "add_margin") {
        list = success
            ? QStringList{"追加功德，稳住阵脚，悬崖勒马未为晚。", "雪中送炭，功德护体，此缘还可续。", "加一分保证金，多一分生机，施主英明。"}
            : QStringList{"功德不足，无法追加。命里有时终须有，命里无时莫强求。", "口袋比脸还干净，拿什么追加？"};
    } else if (actionType == "close_leverage") {
        list = success
            ? QStringList{"舍得舍得，有舍才有得。平仓即是重生。", "断臂求生，亦是智慧。留得青山在，不怕没柴烧。", "昔日杠杆如梦幻泡影，今日平仓得大自在。"}
            : QStringList{"本无杠杆，谈何平仓？施主多虑了。", "空仓如也，无仓可平，不如去幻缘所看看。"};
    } else {
        list = QStringList{"神兽打了个盹，什么也没说。"};
    }

    if (!list.isEmpty()) {
        int idx = QRandomGenerator::global()->bounded(list.size());
        m_beastText->setText(list[idx]);
    }

    // 将 BankDialog 客户区坐标转为全局屏幕坐标（神兽头旁，以右上角为基准）
    double sx = width() / 960.0;
    double sy = height() / 540.0;
    int cw = qRound(240 * sx);
    int cx = qRound(500 * sx) - cw;
    int cy = qRound(136 * sy);
    QPoint globalPos = mapToGlobal(QPoint(cx, cy));
    m_cloudFrame->move(globalPos);

    m_cloudFrame->show();
    m_cloudFrame->raise();
    m_talkTimer->start(3500);
}


// 统一古风通知弹窗
void BankDialog::showToast(const QString& title, const QString& message, const QString& type)
{
    QDialog dialog(this, Qt::Dialog | Qt::FramelessWindowHint);
    dialog.setAttribute(Qt::WA_TranslucentBackground);
    dialog.setFixedSize(400, 220);

    QString borderColor, titleColor, bgColor;
    if (type == "success") {
        borderColor = "rgba(80, 160, 80, 0.8)";
        titleColor = "#81C784";
        bgColor = "rgba(15, 30, 15, 0.95)";
    } else if (type == "error") {
        borderColor = "rgba(200, 80, 80, 0.8)";
        titleColor = "#FF8A80";
        bgColor = "rgba(30, 15, 15, 0.95)";
    } else {
        borderColor = "rgba(200, 160, 80, 0.7)";
        titleColor = "#FFD700";
        bgColor = "rgba(25, 20, 15, 0.95)";
    }

    QWidget *mask = new QWidget(&dialog);
    mask->setGeometry(0, 0, 400, 220);
    mask->setStyleSheet(
        QString("QWidget {"
                "  background-color: %1;"
                "  border: 3px solid %2;"
                "  border-radius: 16px;"
                "}").arg(bgColor).arg(borderColor)
    );

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setSpacing(12);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(24, 20, 24, 20);

    QLabel *titleLabel = new QLabel(title, &dialog);
    titleLabel->setStyleSheet(
        QString("font-size: 20px; font-weight: bold; color: %1;"
                "font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;").arg(titleColor)
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QFrame *line = new QFrame(&dialog);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet(QString("color: %1;").arg(borderColor));
    line->setFixedHeight(2);
    layout->addWidget(line);

    QLabel *msgLabel = new QLabel(message, &dialog);
    msgLabel->setStyleSheet(
        "font-size: 14px; color: #F5F5DC;"
        "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
    );
    msgLabel->setAlignment(Qt::AlignCenter);
    msgLabel->setWordWrap(true);
    layout->addWidget(msgLabel);

    QPushButton *okBtn = new QPushButton("知道了", &dialog);
    okBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(93, 64, 55, 0.88);"
        "  color: #FFD700;"
        "  border: 2px solid rgba(200, 160, 80, 0.7);"
        "  border-radius: 10px;"
        "  padding: 8px 24px;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(200, 160, 80, 0.85);"
        "  color: #3E2723;"
        "}"
    );
    okBtn->setCursor(Qt::PointingHandCursor);
    layout->addWidget(okBtn, 0, Qt::AlignCenter);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();
}
