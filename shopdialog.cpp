#include "shopdialog.h"
#include "ui_shopdialog.h"

ShopDialog::ShopDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ShopDialog)
    , m_wallet(nullptr)
    , m_currentInstrument(Instrument::BasicWoodenFish)
    , m_selectedInstrument(Instrument::BasicWoodenFish)
{
    ui->setupUi(this);
    setWindowTitle("法器阁");

    connect(ui->buyBtn, &QPushButton::clicked, this, &ShopDialog::onBuy);
    connect(ui->equipBtn, &QPushButton::clicked, this, &ShopDialog::onEquip);

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

Instrument ShopDialog::currentInstrument() const
{
    return m_currentInstrument;
}

void ShopDialog::populateShop()
{
    ui->instrumentList->clear();

    QList<Instrument::Type> types = {
        Instrument::BasicWoodenFish,
        Instrument::ElectronicIncense,
        Instrument::TurboWoodenFish,
        Instrument::QuantumPrayerBeads,
        Instrument::AIChantingMachine
    };

    for (Instrument::Type type : types) {
        Instrument inst(type);
        QString itemText = QString("%1 - 价格: %2").arg(inst.name()).arg(inst.price());
        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, (int)type);
        ui->instrumentList->addItem(item);
    }

    ui->instrumentList->setCurrentRow(0);
}

void ShopDialog::onBuy()
{
    if (!m_wallet) return;

    QListWidgetItem* item = ui->instrumentList->currentItem();
    if (!item) return;

    Instrument::Type type = (Instrument::Type)item->data(Qt::UserRole).toInt();
    Instrument inst(type);

    if (m_wallet->merit() >= inst.price()) {
        m_wallet->spend(inst.price());
        m_selectedInstrument = inst;
        ui->statusLabel->setText(QString("已购买: %1").arg(inst.name()));
        ui->equipBtn->setEnabled(true);
        updateWallet();
    } else {
        ui->statusLabel->setText("功德不足!");
    }
}

void ShopDialog::onEquip()
{
    m_currentInstrument = m_selectedInstrument;
    ui->statusLabel->setText(QString("已装备: %1").arg(m_currentInstrument.name()));
    emit instrumentChanged(m_currentInstrument);
}

void ShopDialog::updateWallet()
{
    if (!m_wallet) return;
    ui->meritBalance->setText(QString("功德: %1").arg(m_wallet->merit(), 0, 'f', 2));
}
