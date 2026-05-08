/********************************************************************************
** Form generated from reading UI file 'merithall.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MERITHALL_H
#define UI_MERITHALL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeritHall
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *hudWidget;
    QHBoxLayout *hudLayout;
    QLabel *dateLabel;
    QLabel *meritLabel;
    QLabel *assetLabel;
    QLabel *networthLabel;
    QLabel *yezhangLabel;
    QLabel *eventLabel;
    QWidget *centerWidget;
    QHBoxLayout *centerLayout;
    QWidget *instrumentArea;
    QVBoxLayout *instrumentLayout;
    QLabel *instrumentNameLabel;
    QPushButton *instrumentButton;
    QLabel *feedbackLabel;
    QWidget *statsWidget;
    QVBoxLayout *statsLayout;
    QLabel *clickCountLabel;
    QLabel *autoIncomeLabel;
    QLabel *efficiencyLabel;
    QLabel *inflationLabel;
    QWidget *navWidget;
    QHBoxLayout *navLayout;
    QPushButton *bankBtn;
    QPushButton *exchangeBtn;
    QPushButton *shopBtn;
    QPushButton *achievementBtn;
    QPushButton *yezhangBtn;

    void setupUi(QMainWindow *MeritHall)
    {
        if (MeritHall->objectName().isEmpty())
            MeritHall->setObjectName("MeritHall");
        MeritHall->resize(800, 600);
        centralWidget = new QWidget(MeritHall);
        centralWidget->setObjectName("centralWidget");
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        hudWidget = new QWidget(centralWidget);
        hudWidget->setObjectName("hudWidget");
        hudLayout = new QHBoxLayout(hudWidget);
        hudLayout->setSpacing(6);
        hudLayout->setContentsMargins(11, 11, 11, 11);
        hudLayout->setObjectName("hudLayout");
        hudLayout->setContentsMargins(0, 0, 0, 0);
        dateLabel = new QLabel(hudWidget);
        dateLabel->setObjectName("dateLabel");

        hudLayout->addWidget(dateLabel);

        meritLabel = new QLabel(hudWidget);
        meritLabel->setObjectName("meritLabel");

        hudLayout->addWidget(meritLabel);

        assetLabel = new QLabel(hudWidget);
        assetLabel->setObjectName("assetLabel");

        hudLayout->addWidget(assetLabel);

        networthLabel = new QLabel(hudWidget);
        networthLabel->setObjectName("networthLabel");

        hudLayout->addWidget(networthLabel);

        yezhangLabel = new QLabel(hudWidget);
        yezhangLabel->setObjectName("yezhangLabel");

        hudLayout->addWidget(yezhangLabel);


        verticalLayout_2->addWidget(hudWidget);

        eventLabel = new QLabel(centralWidget);
        eventLabel->setObjectName("eventLabel");
        eventLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(eventLabel);

        centerWidget = new QWidget(centralWidget);
        centerWidget->setObjectName("centerWidget");
        centerLayout = new QHBoxLayout(centerWidget);
        centerLayout->setSpacing(6);
        centerLayout->setContentsMargins(11, 11, 11, 11);
        centerLayout->setObjectName("centerLayout");
        centerLayout->setContentsMargins(0, 0, 0, 0);
        instrumentArea = new QWidget(centerWidget);
        instrumentArea->setObjectName("instrumentArea");
        instrumentLayout = new QVBoxLayout(instrumentArea);
        instrumentLayout->setSpacing(6);
        instrumentLayout->setContentsMargins(11, 11, 11, 11);
        instrumentLayout->setObjectName("instrumentLayout");
        instrumentLayout->setContentsMargins(0, 0, 0, 0);
        instrumentNameLabel = new QLabel(instrumentArea);
        instrumentNameLabel->setObjectName("instrumentNameLabel");
        instrumentNameLabel->setAlignment(Qt::AlignCenter);

        instrumentLayout->addWidget(instrumentNameLabel);

        instrumentButton = new QPushButton(instrumentArea);
        instrumentButton->setObjectName("instrumentButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(instrumentButton->sizePolicy().hasHeightForWidth());
        instrumentButton->setSizePolicy(sizePolicy);

        instrumentLayout->addWidget(instrumentButton);

        feedbackLabel = new QLabel(instrumentArea);
        feedbackLabel->setObjectName("feedbackLabel");
        feedbackLabel->setAlignment(Qt::AlignCenter);

        instrumentLayout->addWidget(feedbackLabel);


        centerLayout->addWidget(instrumentArea);

        statsWidget = new QWidget(centerWidget);
        statsWidget->setObjectName("statsWidget");
        statsLayout = new QVBoxLayout(statsWidget);
        statsLayout->setSpacing(6);
        statsLayout->setContentsMargins(11, 11, 11, 11);
        statsLayout->setObjectName("statsLayout");
        statsLayout->setContentsMargins(0, 0, 0, 0);
        clickCountLabel = new QLabel(statsWidget);
        clickCountLabel->setObjectName("clickCountLabel");

        statsLayout->addWidget(clickCountLabel);

        autoIncomeLabel = new QLabel(statsWidget);
        autoIncomeLabel->setObjectName("autoIncomeLabel");

        statsLayout->addWidget(autoIncomeLabel);

        efficiencyLabel = new QLabel(statsWidget);
        efficiencyLabel->setObjectName("efficiencyLabel");

        statsLayout->addWidget(efficiencyLabel);

        inflationLabel = new QLabel(statsWidget);
        inflationLabel->setObjectName("inflationLabel");

        statsLayout->addWidget(inflationLabel);


        centerLayout->addWidget(statsWidget);


        verticalLayout_2->addWidget(centerWidget);

        navWidget = new QWidget(centralWidget);
        navWidget->setObjectName("navWidget");
        navLayout = new QHBoxLayout(navWidget);
        navLayout->setSpacing(6);
        navLayout->setContentsMargins(11, 11, 11, 11);
        navLayout->setObjectName("navLayout");
        navLayout->setContentsMargins(0, 0, 0, 0);
        bankBtn = new QPushButton(navWidget);
        bankBtn->setObjectName("bankBtn");

        navLayout->addWidget(bankBtn);

        exchangeBtn = new QPushButton(navWidget);
        exchangeBtn->setObjectName("exchangeBtn");

        navLayout->addWidget(exchangeBtn);

        shopBtn = new QPushButton(navWidget);
        shopBtn->setObjectName("shopBtn");

        navLayout->addWidget(shopBtn);

        achievementBtn = new QPushButton(navWidget);
        achievementBtn->setObjectName("achievementBtn");

        navLayout->addWidget(achievementBtn);

        yezhangBtn = new QPushButton(navWidget);
        yezhangBtn->setObjectName("yezhangBtn");

        navLayout->addWidget(yezhangBtn);


        verticalLayout_2->addWidget(navWidget);

        MeritHall->setCentralWidget(centralWidget);

        retranslateUi(MeritHall);

        QMetaObject::connectSlotsByName(MeritHall);
    } // setupUi

    void retranslateUi(QMainWindow *MeritHall)
    {
        MeritHall->setWindowTitle(QCoreApplication::translate("MeritHall", "\345\212\237\345\276\267\345\240\202", nullptr));
        dateLabel->setText(QCoreApplication::translate("MeritHall", "2026\345\271\26401\346\234\21001\346\227\245", nullptr));
        dateLabel->setStyleSheet(QCoreApplication::translate("MeritHall", "font-weight: bold;", nullptr));
        meritLabel->setText(QCoreApplication::translate("MeritHall", "\345\212\237\345\276\267: 0", nullptr));
        assetLabel->setText(QCoreApplication::translate("MeritHall", "\350\265\204\344\272\247\345\270\202\345\200\274: 0", nullptr));
        networthLabel->setText(QCoreApplication::translate("MeritHall", "\345\207\200\350\265\204\344\272\247: 0", nullptr));
        yezhangLabel->setText(QCoreApplication::translate("MeritHall", "\344\270\232\351\232\234: 0", nullptr));
        eventLabel->setText(QCoreApplication::translate("MeritHall", "\345\270\202\345\234\272\345\271\263\347\250\263\350\277\220\350\241\214\344\270\255...", nullptr));
        instrumentNameLabel->setText(QCoreApplication::translate("MeritHall", "\345\237\272\347\241\200\346\234\250\351\261\274", nullptr));
        instrumentButton->setText(QCoreApplication::translate("MeritHall", "\360\237\252\265 \346\225\262\345\207\273\346\263\225\345\231\250", nullptr));
        feedbackLabel->setText(QString());
        clickCountLabel->setText(QCoreApplication::translate("MeritHall", "\344\273\212\346\227\245\347\202\271\345\207\273: 0", nullptr));
        autoIncomeLabel->setText(QCoreApplication::translate("MeritHall", "\350\207\252\345\212\250\346\224\266\347\233\212: 0/\347\247\222", nullptr));
        efficiencyLabel->setText(QCoreApplication::translate("MeritHall", "\346\225\210\347\216\207\347\263\273\346\225\260: 100%", nullptr));
        inflationLabel->setText(QCoreApplication::translate("MeritHall", "\351\200\232\350\203\200: 0.00%/\346\227\245", nullptr));
        bankBtn->setText(QCoreApplication::translate("MeritHall", "\360\237\217\246 \345\226\204\350\264\242\345\217\270", nullptr));
        exchangeBtn->setText(QCoreApplication::translate("MeritHall", "\360\237\223\210 \345\271\273\347\274\230\346\211\200", nullptr));
        shopBtn->setText(QCoreApplication::translate("MeritHall", "\360\237\233\222 \346\263\225\345\231\250\351\230\201", nullptr));
        achievementBtn->setText(QCoreApplication::translate("MeritHall", "\360\237\217\206 \346\210\220\345\260\261\346\246\234", nullptr));
        yezhangBtn->setText(QCoreApplication::translate("MeritHall", "\360\237\231\217 \345\277\217\346\202\224", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeritHall: public Ui_MeritHall {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MERITHALL_H
