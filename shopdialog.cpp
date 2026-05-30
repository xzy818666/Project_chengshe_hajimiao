#include "shopdialog.h"
#include "ui_shopdialog.h"

ShopDialog::ShopDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ShopDialog)
    , m_wallet(nullptr)
    , m_currentCloudInstrument(Instrument::BasicWoodenFish)
    , m_selectedInstrument(Instrument::BasicWoodenFish)
{
    ui->setupUi(this);
    setWindowTitle("法器阁");

    // 基础木鱼默认已拥有且已装备
    m_ownedTypes.insert((int)Instrument::BasicWoodenFish);

    connect(ui->buyBtn, &QPushButton::clicked, this, &ShopDialog::onBuy);
    connect(ui->equipBtn, &QPushButton::clicked, this, &ShopDialog::onEquip);
    connect(ui->instrumentList, &QListWidget::currentRowChanged,
            this, &ShopDialog::onSelectionChanged);

    populateShop();
}

ShopDialog::~ShopDialog()
{
    delete ui;
}

void ShopDialog::setWallet(Wallet* wallet)
{
    m_wallet = wallet;
    connect(m_wallet, &Wallet::meritChanged, this, &ShopDialog::updateWallet);
    updateWallet();
}

void ShopDialog::populateShop()
{
    ui->instrumentList->clear();

    QList<Instrument::Type> types = {
        Instrument::BasicWoodenFish,
        Instrument::BasicIncense,
        Instrument::ElectronicIncense,
        Instrument::TurboWoodenFish,
        Instrument::QuantumPrayerBeads,
        Instrument::AIChantingMachine
    };

    for (Instrument::Type type : types) {
        Instrument inst(type);
        QString status = m_ownedTypes.contains((int)type) ? "【已拥有】" : "";
        QString itemText = QString("%1%2 - 价格: %3")
                               .arg(inst.name())
                               .arg(status)
                               .arg(inst.price());
        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, (int)type);
        ui->instrumentList->addItem(item);
    }

    ui->instrumentList->setCurrentRow(0);
    onSelectionChanged();
}

void ShopDialog::onSelectionChanged()
{
    QListWidgetItem* item = ui->instrumentList->currentItem();
    if (!item) {
        ui->equipBtn->setEnabled(false);
        return;
    }

    Instrument::Type type = (Instrument::Type)item->data(Qt::UserRole).toInt();
    m_selectedInstrument = Instrument(type);
    refreshStatus();
}

void ShopDialog::refreshStatus()
{
    bool owned = m_ownedTypes.contains((int)m_selectedInstrument.type());
    ui->equipBtn->setEnabled(owned);

    if (m_selectedInstrument.isLotusInstrument() && owned) {
        bool equipped = m_equippedLotusTypes.contains((int)m_selectedInstrument.type());
        ui->equipBtn->setText(equipped ? "卸下" : "装备");
    } else {
        ui->equipBtn->setText("装备");
    }

    if (!owned) {
        ui->statusLabel->setText(QString("尚未拥有 %1，请先购买")
                                     .arg(m_selectedInstrument.name()));
    } else {
        ui->statusLabel->setText(QString("已选中: %1").arg(m_selectedInstrument.name()));
    }
}

void ShopDialog::onBuy()
{
    if (!m_wallet) return;

    QListWidgetItem* item = ui->instrumentList->currentItem();
    if (!item) return;

    Instrument::Type type = (Instrument::Type)item->data(Qt::UserRole).toInt();
    Instrument inst(type);

    if (m_ownedTypes.contains((int)type)) {
        ui->statusLabel->setText("已拥有该法器");
        return;
    }

    if (m_wallet->merit() >= inst.price()) {
        m_wallet->spend(inst.price());
        m_ownedTypes.insert((int)type);
        m_selectedInstrument = inst;
        ui->statusLabel->setText(QString("已购买: %1").arg(inst.name()));
        populateShop(); // 刷新列表显示【已拥有】
        updateWallet();
    } else {
        ui->statusLabel->setText("功德不足!");
    }
}

void ShopDialog::onEquip()
{
    QListWidgetItem* item = ui->instrumentList->currentItem();
    if (!item) return;

    Instrument::Type type = (Instrument::Type)item->data(Qt::UserRole).toInt();
    Instrument inst(type);

    if (!m_ownedTypes.contains((int)type)) return;

    if (inst.isCloudInstrument()) {
        m_currentCloudInstrument = inst;
        ui->statusLabel->setText(QString("已装备主动法器: %1").arg(inst.name()));
        emit cloudInstrumentChanged(inst);
    } else if (inst.isLotusInstrument()) {
        int typeId = (int)type;
        if (m_equippedLotusTypes.contains(typeId)) {
            m_equippedLotusTypes.remove(typeId);
            ui->statusLabel->setText(QString("已卸下辅助法器: %1").arg(inst.name()));
        } else {
            m_equippedLotusTypes.insert(typeId);
            ui->statusLabel->setText(QString("已装备辅助法器: %1").arg(inst.name()));
        }
        emit lotusInstrumentToggled(inst);
    }
    refreshStatus();
}

void ShopDialog::updateWallet()
{
    if (!m_wallet) return;
    ui->meritBalance->setText(QString("功德: %1").arg(m_wallet->merit(), 0, 'f', 2));
}
