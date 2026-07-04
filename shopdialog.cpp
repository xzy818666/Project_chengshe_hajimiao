#include "shopdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QPixmap>
#include <QMouseEvent>


ShopDialog::ShopDialog(QWidget *parent)
    : QDialog(parent)
    , m_wallet(nullptr)
    , m_currentCloudInstrument(Instrument::BasicWoodenFish)
    , m_selectedInstrument(Instrument::BasicWoodenFish)
    , m_centerGlow(nullptr)
    , m_centerImage(nullptr)
    , m_nameLabel(nullptr)
    , m_descLabel(nullptr)
    , m_priceLabel(nullptr)
    , m_buyBtn(nullptr)
    , m_equipBtn(nullptr)
    , m_bottomPanel(nullptr)
{
    setWindowTitle("法器阁");

    QSize sz = parent ? parent->size() : QSize(1600, 900);
    resize(sz.width(), sz.height());
    setMinimumSize(800, 450);

    // 背景图
    QPixmap bg(":/images/instrument_shop.png");
    bg = bg.scaled(sz.width(), sz.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(bg));
    setPalette(palette);
    setAutoFillBackground(true);

    // 基础木鱼默认已拥有，其余后加辅助法器需购买
    m_ownedTypes.insert((int)Instrument::BasicWoodenFish);

    setupUI();
    updateBottomPanel();

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
    connect(backBtn, &QPushButton::clicked, this, &ShopDialog::accept);
}

ShopDialog::~ShopDialog()
{
}

void ShopDialog::setWallet(Wallet* wallet)
{
    m_wallet = wallet;
    if (m_wallet) {
        connect(m_wallet, &Wallet::meritChanged, this, [this]() {
            updateBottomPanel();
        });
    }
    updateBottomPanel();
}

void ShopDialog::setupUI()
{
    // 左柜子缩略图（主动法器）
    QList<Instrument::Type> cloudTypes = {
        Instrument::BasicWoodenFish,
        Instrument::TurboWoodenFish,
        Instrument::QuantumPrayerBeads,
        Instrument::AllSeeingEye
    };
    for (Instrument::Type type : cloudTypes) {
        QLabel* label = new QLabel(this);
        label->setScaledContents(true);
        label->setCursor(Qt::PointingHandCursor);
        label->setProperty("instrumentType", (int)type);
        label->installEventFilter(this);
        m_leftThumbs.append({type, label});
    }

    // 右柜子缩略图（辅助法器）
    QList<Instrument::Type> lotusTypes = {
        Instrument::BasicIncense,
        Instrument::ElectronicIncense,
        Instrument::AIChantingMachine,
        Instrument::Cassock,
        Instrument::Compass,
        Instrument::Fuchen,
        Instrument::Ruyi
    };
    for (Instrument::Type type : lotusTypes) {
        QLabel* label = new QLabel(this);
        label->setScaledContents(true);
        label->setCursor(Qt::PointingHandCursor);
        label->setProperty("instrumentType", (int)type);
        label->installEventFilter(this);
        m_rightThumbs.append({type, label});
    }

    // 中央发光背景（radiance.png）
    m_centerGlow = new QLabel(this);
    m_centerGlow->setScaledContents(true);
    m_centerGlow->setPixmap(QPixmap(":/images/radiance.png"));
    m_centerGlow->setAttribute(Qt::WA_TransparentForMouseEvents);

    // 中央大图（法器展示）
    m_centerImage = new QLabel(this);
    m_centerImage->setScaledContents(true);
    m_centerImage->setAlignment(Qt::AlignCenter);
    m_centerImage->setAttribute(Qt::WA_TransparentForMouseEvents);

    // 底部面板
    m_bottomPanel = new QWidget(this);
    m_bottomPanel->setStyleSheet(
        "QWidget {"
        "  background-color: rgba(35, 25, 15, 0.92);"
        "  border: 2px solid rgba(200, 160, 80, 0.65);"
        "  border-radius: 14px;"
        "}"
    );
    QVBoxLayout* panelLayout = new QVBoxLayout(m_bottomPanel);
    panelLayout->setSpacing(8);
    panelLayout->setContentsMargins(24, 14, 24, 14);

    m_nameLabel = new QLabel(m_bottomPanel);
    m_nameLabel->setStyleSheet(
        "color: #FFD700; font-size: 24px; font-weight: bold;"
        "font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
    );
    panelLayout->addWidget(m_nameLabel);

    m_descLabel = new QLabel(m_bottomPanel);
    m_descLabel->setStyleSheet(
        "color: #F5F5DC; font-size: 14px;"
        "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
    );
    m_descLabel->setWordWrap(true);
    panelLayout->addWidget(m_descLabel);

    m_priceLabel = new QLabel(m_bottomPanel);
    m_priceLabel->setStyleSheet(
        "color: #FFA500; font-size: 15px; font-weight: bold;"
        "font-family: 'STKaiti', 'KaiTi', 'SimSun', serif;"
    );
    panelLayout->addWidget(m_priceLabel);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    m_buyBtn = new QPushButton("购买", m_bottomPanel);
    m_buyBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #5D4037;"
        "  color: #FFD700;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "  font-size: 15px;"
        "  padding: 10px 32px;"
        "  border: 2px solid #FFD700;"
        "  border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #FFD700;"
        "  color: #3E2723;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #4A4A4A;"
        "  color: #888888;"
        "  border: 2px solid #666666;"
        "}"
    );
    m_equipBtn = new QPushButton("装备", m_bottomPanel);
    m_equipBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #2E4D33;"
        "  color: #FFF8E7;"
        "  font-weight: bold;"
        "  font-family: 'STXingkai', 'STKaiti', 'LiSu', 'KaiTi', 'SimSun', serif;"
        "  font-size: 15px;"
        "  padding: 10px 32px;"
        "  border: 2px solid #81C784;"
        "  border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #81C784;"
        "  color: #1B331F;"
        "}"
    );
    btnLayout->addStretch();
    btnLayout->addWidget(m_buyBtn);
    btnLayout->addWidget(m_equipBtn);
    btnLayout->addStretch();
    panelLayout->addLayout(btnLayout);

    connect(m_buyBtn, &QPushButton::clicked, this, &ShopDialog::onBuy);
    connect(m_equipBtn, &QPushButton::clicked, this, &ShopDialog::onEquip);

    updateLayout();

    // 初始隐藏中央展示区和发光背景
    m_centerGlow->hide();
    m_centerImage->hide();
}

void ShopDialog::updateLayout()
{
    int w = width();
    int h = height();

    // 基于原图 1672×941 的缩放比例
    double sx = w / 1672.0;
    double sy = h / 941.0;
    int thumbSize = qMin(120, static_cast<int>(qMin(130 * sx, 130 * sy)));

    // 左柜子坐标（原图绝对像素）
    static const int LEFT_COLS[3] = {125, 270, 420};
    static const int ROWS[3] = {333, 473, 610};

    // 左柜子（主动法器）：3 个按列优先放在第一列
    for (int i = 0; i < m_leftThumbs.size(); ++i) {
        int col = i % 3;
        int row = i / 3;
        int x = qRound(LEFT_COLS[col] * sx) - thumbSize / 2;
        int y = qRound(ROWS[row] * sy) - thumbSize / 2;
        m_leftThumbs[i].label->setGeometry(x, y, thumbSize, thumbSize);
        QPixmap pix(thumbPath(m_leftThumbs[i].type));
        if (!pix.isNull()) {
            m_leftThumbs[i].label->setPixmap(pix);
        }
    }

    // 右柜子坐标（原图绝对像素）
    static const int RIGHT_COLS[3] = {1250, 1400, 1550};

    // 右柜子（辅助法器）：7 个按列优先填满
    for (int i = 0; i < m_rightThumbs.size(); ++i) {
        int col = i / 3;
        int row = i % 3;
        int x = qRound(RIGHT_COLS[col] * sx) - thumbSize / 2;
        int y = qRound(ROWS[row] * sy) - thumbSize / 2;
        m_rightThumbs[i].label->setGeometry(x, y, thumbSize, thumbSize);
        QPixmap pix(thumbPath(m_rightThumbs[i].type));
        if (!pix.isNull()) {
            m_rightThumbs[i].label->setPixmap(pix);
        }
    }

    // 中央发光背景（比法器图大，放在下层）
    int glowSize = qMin(560, qMin(w / 2, h * 2 / 3));
    int glowX = w / 2 - glowSize / 2;
    int glowY = qRound(h * 0.40) - glowSize / 2;
    m_centerGlow->setGeometry(glowX, glowY, glowSize, glowSize);

    // 中央大图（法器展示）
    int centerSize = qMin(420, qMin(w / 3, h / 2));
    int centerX = w / 2 - centerSize / 2;
    int centerY = qRound(h * 0.40) - centerSize / 2;
    m_centerImage->setGeometry(centerX, centerY, centerSize, centerSize);

    // 底部面板
    int panelW = qRound(w * 0.55);
    int panelH = qRound(h * 0.20);
    int panelX = w / 2 - panelW / 2;
    int panelY = qRound(h * 0.74);
    m_bottomPanel->setGeometry(panelX, panelY, panelW, panelH);
}

void ShopDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    updateLayout();
}

bool ShopDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel* label = qobject_cast<QLabel*>(obj);
        if (label) {
            int type = label->property("instrumentType").toInt();
            Instrument::Type clickedType = (Instrument::Type)type;

            if (m_centerVisible && m_selectedInstrument.type() == clickedType) {
                // 再次点击同一个，隐藏中央展示
                m_centerVisible = false;
                m_centerGlow->hide();
                m_centerImage->hide();
            } else {
                // 点击不同法器，或当前未显示，则显示
                m_centerVisible = true;
                m_selectedInstrument = Instrument(clickedType);
                updateCenterDisplay();
            }
            updateBottomPanel();
            return true;
        }
    }
    return QDialog::eventFilter(obj, event);
}

void ShopDialog::updateCenterDisplay()
{
    if (!m_centerVisible) {
        m_centerGlow->hide();
        m_centerImage->hide();
        return;
    }

    QString path = fullPath(m_selectedInstrument.type());
    if (!path.isEmpty()) {
        QPixmap pix(path);
        if (!pix.isNull()) {
            m_centerImage->setPixmap(pix);
        }
    }
    m_centerGlow->show();
    m_centerGlow->raise();
    m_centerImage->show();
    m_centerImage->raise();
}

void ShopDialog::updateBottomPanel()
{
    Instrument inst = m_selectedInstrument;
    m_nameLabel->setText(inst.name());

    QString desc;
    switch (inst.type()) {
    case Instrument::BasicWoodenFish:
        desc = "新手默认装备。每击 +1 功德，朴实无华，大道至简。";
        break;
    case Instrument::TurboWoodenFish:
        desc = "涡轮增压木鱼。每击 +5 功德，敲出赛博时代的节奏感。";
        break;
    case Instrument::QuantumPrayerBeads:
        desc = "量子佛珠。每击 +2 功德，10% 概率触发十倍暴击，功德叠加态崩塌。";
        break;
    case Instrument::BasicIncense:
        desc = "基础烧香。辅助法器，自动收益 +0.5/秒，持续 60 秒，青烟袅袅。";
        break;
    case Instrument::ElectronicIncense:
        desc = "电子烧香。辅助法器，自动收益 +1/秒，持续 60 秒，赛博香火不断。";
        break;
    case Instrument::AIChantingMachine:
        desc = "AI 诵经机。辅助法器，自动收益 +10/秒，但每秒需 1 功德维护费，算法普度众生。";
        break;
    case Instrument::Cassock:
        desc = "袈裟。佛门法衣，辅助法器，效率系数 +10%，佛光普照。";
        break;
    case Instrument::Compass:
        desc = "罗盘。风水定方位，辅助法器，自动收益 +0.3/秒，指引财运。";
        break;
    case Instrument::Fuchen:
        desc = "拂尘。扫除烦恼尘垢，辅助法器，点击收益 +0.5/击，清净心明。";
        break;
    case Instrument::Ruyi:
        desc = "如意。万事如意，辅助法器，暴击概率 +5%，心想事成。";
        break;
    case Instrument::AllSeeingEye:
        desc = "全视之眼。每击 +3 功德，5% 概率触发五倍暴击，洞察因果。";
        break;
    default:
        desc = "";
    }
    m_descLabel->setText(desc);

    bool owned = m_ownedTypes.contains((int)inst.type());
    if (owned) {
        m_priceLabel->setText("状态: 已拥有");
        m_buyBtn->setEnabled(false);
    } else {
        m_priceLabel->setText(QString("价格: %1 功德").arg(inst.price()));
        m_buyBtn->setEnabled(m_wallet && m_wallet->merit() >= inst.price());
    }

    if (inst.isLotusInstrument() && owned) {
        bool equipped = m_equippedLotusTypes.contains((int)inst.type());
        m_equipBtn->setText(equipped ? "卸下" : "装备");
    } else {
        m_equipBtn->setText("装备");
    }
    m_equipBtn->setEnabled(owned);
}

QString ShopDialog::thumbPath(Instrument::Type type) const
{
    switch (type) {
    case Instrument::BasicWoodenFish:    return ":/images/basic_muyu_unstriked.png";
    case Instrument::TurboWoodenFish:    return ":/images/turbo_muyu_unstriked.png";
    case Instrument::QuantumPrayerBeads: return ":/images/quantum_beads_unstriked.png";
    case Instrument::BasicIncense:       return ":/images/basic_joss_stick.png";
    case Instrument::ElectronicIncense:  return ":/images/electronic_incense.png";
    case Instrument::AIChantingMachine:  return ":/images/ai_chanting.png";
    case Instrument::Cassock:            return ":/images/cassock.png";
    case Instrument::Compass:            return ":/images/compass.png";
    case Instrument::Fuchen:             return ":/images/fuchen.png";
    case Instrument::Ruyi:               return ":/images/ruyi.png";
    case Instrument::AllSeeingEye:       return ":/images/AllSeeingEye_closed.png";
    default: return "";
    }
}

QString ShopDialog::fullPath(Instrument::Type type) const
{
    switch (type) {
    case Instrument::BasicWoodenFish:    return ":/images/basic_muyu_unstriked.png";
    case Instrument::TurboWoodenFish:    return ":/images/turbo_muyu_unstriked.png";
    case Instrument::QuantumPrayerBeads: return ":/images/quantum_beads_unstriked.png";
    case Instrument::BasicIncense:       return ":/images/basic_joss_stick.png";
    case Instrument::ElectronicIncense:  return ":/images/electronic_incense.png";
    case Instrument::AIChantingMachine:  return ":/images/ai_chanting.png";
    case Instrument::Cassock:            return ":/images/cassock.png";
    case Instrument::Compass:            return ":/images/compass.png";
    case Instrument::Fuchen:             return ":/images/fuchen.png";
    case Instrument::Ruyi:               return ":/images/ruyi.png";
    case Instrument::AllSeeingEye:       return ":/images/AllSeeingEye_closed.png";
    default: return "";
    }
}

void ShopDialog::onBuy()
{
    if (!m_wallet) return;

    Instrument inst = m_selectedInstrument;
    if (m_ownedTypes.contains((int)inst.type())) {
        m_priceLabel->setText("已拥有该法器");
        return;
    }

    if (m_wallet->merit() >= inst.price()) {
        m_wallet->spend(inst.price());
        m_ownedTypes.insert((int)inst.type());
        m_priceLabel->setText(QString("已购买: %1").arg(inst.name()));
        updateBottomPanel();
    } else {
        m_priceLabel->setText("功德不足!");
    }
}

void ShopDialog::onEquip()
{
    Instrument inst = m_selectedInstrument;
    if (!m_ownedTypes.contains((int)inst.type())) return;

    if (inst.isCloudInstrument()) {
        m_currentCloudInstrument = inst;
        emit cloudInstrumentChanged(inst);
    } else if (inst.isLotusInstrument()) {
        int typeId = (int)inst.type();
        if (m_equippedLotusTypes.contains(typeId)) {
            m_equippedLotusTypes.remove(typeId);
        } else {
            m_equippedLotusTypes.insert(typeId);
        }
        emit lotusInstrumentToggled(inst);
    }
    updateBottomPanel();
}
