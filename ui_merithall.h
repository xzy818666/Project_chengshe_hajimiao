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
#include <QtWidgets/QSpacerItem>
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
    QLabel *meritLabel;
    QLabel *assetLabel;
    QLabel *networthLabel;
    QLabel *yezhangLabel;
    QLabel *eventLabel;
    QSpacerItem *topSpacer;
    QWidget *centerWidget;
    QVBoxLayout *centerLayout;
    QLabel *instrumentIconLabel;
    QLabel *instrumentNameLabel;
    QPushButton *instrumentButton;
    QLabel *feedbackLabel;
    QSpacerItem *bottomSpacer;
    QWidget *navWidget;
    QHBoxLayout *navLayout;
    QPushButton *bankBtn;
    QPushButton *exchangeBtn;
    QPushButton *shopBtn;
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

        topSpacer = new QSpacerItem(20, 100, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(topSpacer);

        centerWidget = new QWidget(centralWidget);
        centerWidget->setObjectName("centerWidget");
        centerLayout = new QVBoxLayout(centerWidget);
        centerLayout->setSpacing(6);
        centerLayout->setContentsMargins(11, 11, 11, 11);
        centerLayout->setObjectName("centerLayout");
        centerLayout->setContentsMargins(0, 0, 0, 0);
        instrumentIconLabel = new QLabel(centerWidget);
        instrumentIconLabel->setObjectName("instrumentIconLabel");
        instrumentIconLabel->setAlignment(Qt::AlignCenter);

        centerLayout->addWidget(instrumentIconLabel);

        instrumentNameLabel = new QLabel(centerWidget);
        instrumentNameLabel->setObjectName("instrumentNameLabel");
        instrumentNameLabel->setAlignment(Qt::AlignCenter);

        centerLayout->addWidget(instrumentNameLabel);

        instrumentButton = new QPushButton(centerWidget);
        instrumentButton->setObjectName("instrumentButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(instrumentButton->sizePolicy().hasHeightForWidth());
        instrumentButton->setSizePolicy(sizePolicy);

        centerLayout->addWidget(instrumentButton);

        feedbackLabel = new QLabel(centerWidget);
        feedbackLabel->setObjectName("feedbackLabel");
        feedbackLabel->setAlignment(Qt::AlignCenter);

        centerLayout->addWidget(feedbackLabel);


        verticalLayout_2->addWidget(centerWidget);

        bottomSpacer = new QSpacerItem(20, 50, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(bottomSpacer);

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
        meritLabel->setText(QCoreApplication::translate("MeritHall", "\345\212\237\345\276\267: 0", nullptr));
        assetLabel->setText(QCoreApplication::translate("MeritHall", "\350\265\204\344\272\247\345\270\202\345\200\274: 0", nullptr));
        networthLabel->setText(QCoreApplication::translate("MeritHall", "\345\207\200\350\265\204\344\272\247: 0", nullptr));
        yezhangLabel->setText(QCoreApplication::translate("MeritHall", "\344\270\232\351\232\234: 0", nullptr));
        eventLabel->setText(QCoreApplication::translate("MeritHall", "\345\270\202\345\234\272\345\271\263\347\250\263\350\277\220\350\241\214\344\270\255...", nullptr));
        instrumentNameLabel->setText(QCoreApplication::translate("MeritHall", "\345\237\272\347\241\200\346\234\250\351\261\274", nullptr));
        instrumentButton->setText(QCoreApplication::translate("MeritHall", "\360\237\252\265 \346\225\262\345\207\273\346\263\225\345\231\250", nullptr));
        feedbackLabel->setText(QString());
        bankBtn->setText(QCoreApplication::translate("MeritHall", "\360\237\217\246 \345\226\204\350\264\242\345\217\270", nullptr));
        exchangeBtn->setText(QCoreApplication::translate("MeritHall", "\360\237\223\210 \345\271\273\347\274\230\346\211\200", nullptr));
        shopBtn->setText(QCoreApplication::translate("MeritHall", "\360\237\233\222 \346\263\225\345\231\250\351\230\201", nullptr));
        yezhangBtn->setText(QCoreApplication::translate("MeritHall", "\360\237\231\217 \345\277\217\346\202\224", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MeritHall: public Ui_MeritHall {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MERITHALL_H
