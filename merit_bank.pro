QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    meritbank.cpp \
    wallet.cpp \
    asset.cpp \
    meritindex.cpp \
    karmabond.cpp \
    dharmafund.cpp \
    samsarafutures.cpp \
    instrument.cpp \
    merithall.cpp \
    bankdialog.cpp \
    exchangedialog.cpp \
    shopdialog.cpp \
    marketevent.cpp

HEADERS += \
    wallet.h \
    asset.h \
    meritindex.h \
    karmabond.h \
    dharmafund.h \
    samsarafutures.h \
    instrument.h \
    merithall.h \
    bankdialog.h \
    exchangedialog.h \
    shopdialog.h \
    marketevent.h

FORMS += \
    merithall.ui \
    bankdialog.ui \
    exchangedialog.ui \
    shopdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
