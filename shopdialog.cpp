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

    // 基础木鱼默认已拥有且已装备
    m_ownedTypes.insert((int)Instrument::BasicWoodenFish);

    setupUI();
    updateBottomPanel();
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
        Instrument::QuantumPrayerBeads
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
        Instrument::AIChantingMachine
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
        "  background-color: rgba(60, 40, 20, 0.88);"
        "  border: 2px solid rgba(200, 160, 80, 0.6);"
        "  border-radius: 12px;"
        "}"
    );
    QVBoxLayout* panelLayout = new QVBoxLayout(m_bottomPanel);
    panelLayout->setSpacing(6);
    panelLayout->setContentsMargins(20, 12, 20, 12);

    m_nameLabel = new QLabel(m_bottomPanel);
    m_nameLabel->setStyleSheet("color: #FFD700; font-size: 22px; font-weight: bold;");
    panelLayout->addWidget(m_nameLabel);

    m_descLabel = new QLabel(m_bottomPanel);
    m_descLabel->setStyleSheet("color: #F5F5DC; font-size: 14px;");
    m_descLabel->setWordWrap(true);
    panelLayout->addWidget(m_descLabel);

    m_priceLabel = new QLabel(m_bottomPanel);
    m_priceLabel->setStyleSheet("color: #FFA500; font-size: 14px;");
    panelLayout->addWidget(m_priceLabel);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    m_buyBtn = new QPushButton("购买", m_bottomPanel);
    m_buyBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #8B4513;"
        "  color: #FFF8E7;"
        "  font-weight: bold;"
        "  padding: 8px 24px;"
        "  border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #A0522D;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #555555;"
        "  color: #AAAAAA;"
        "}"
    );
    m_equipBtn = new QPushButton("装备", m_bottomPanel);
    m_equipBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #2E7D32;"
        "  color: white;"
        "  font-weight: bold;"
        "  padding: 8px 24px;"
        "  border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #388E3C;"
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

    // 缩略图大小（根据窗口自适应）
    int thumbSize = qMin(100, qMin(w / 10, h / 8));

    // 左柜子（主动法器）：放在左柜子中间列，垂直均匀分布
    for (int i = 0; i < m_leftThumbs.size(); ++i) {
        int x = qRound(w * 0.17) - thumbSize / 2;
        int y = qRound(h * (0.30 + i * 0.18)) - thumbSize / 2;
        m_leftThumbs[i].label->setGeometry(x, y, thumbSize, thumbSize);
        QPixmap pix(thumbPath(m_leftThumbs[i].type));
        if (!pix.isNull()) {
            m_leftThumbs[i].label->setPixmap(pix);
        }
    }

    // 右柜子（辅助法器）
    for (int i = 0; i < m_rightThumbs.size(); ++i) {
        int x = qRound(w * 0.83) - thumbSize / 2;
        int y = qRound(h * (0.30 + i * 0.18)) - thumbSize / 2;
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
