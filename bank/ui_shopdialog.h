/********************************************************************************
** Form generated from reading UI file 'shopdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOPDIALOG_H
#define UI_SHOPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ShopDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *meritBalance;
    QListWidget *instrumentList;
    QLabel *statusLabel;
    QHBoxLayout *buttonLayout;
    QPushButton *buyBtn;
    QPushButton *equipBtn;

    void setupUi(QDialog *ShopDialog)
    {
        if (ShopDialog->objectName().isEmpty())
            ShopDialog->setObjectName("ShopDialog");
        ShopDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(ShopDialog);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        meritBalance = new QLabel(ShopDialog);
        meritBalance->setObjectName("meritBalance");

        verticalLayout->addWidget(meritBalance);

        instrumentList = new QListWidget(ShopDialog);
        instrumentList->setObjectName("instrumentList");

        verticalLayout->addWidget(instrumentList);

        statusLabel = new QLabel(ShopDialog);
        statusLabel->setObjectName("statusLabel");

        verticalLayout->addWidget(statusLabel);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(6);
        buttonLayout->setObjectName("buttonLayout");
        buyBtn = new QPushButton(ShopDialog);
        buyBtn->setObjectName("buyBtn");

        buttonLayout->addWidget(buyBtn);

        equipBtn = new QPushButton(ShopDialog);
        equipBtn->setObjectName("equipBtn");
        equipBtn->setEnabled(false);

        buttonLayout->addWidget(equipBtn);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(ShopDialog);

        QMetaObject::connectSlotsByName(ShopDialog);
    } // setupUi

    void retranslateUi(QDialog *ShopDialog)
    {
        ShopDialog->setWindowTitle(QCoreApplication::translate("ShopDialog", "\346\263\225\345\231\250\351\230\201", nullptr));
        meritBalance->setText(QCoreApplication::translate("ShopDialog", "\345\212\237\345\276\267: 0", nullptr));
        statusLabel->setText(QString());
        buyBtn->setText(QCoreApplication::translate("ShopDialog", "\350\264\255\344\271\260", nullptr));
        equipBtn->setText(QCoreApplication::translate("ShopDialog", "\350\243\205\345\244\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShopDialog: public Ui_ShopDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOPDIALOG_H
