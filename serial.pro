QT       += core gui network
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    acceuil.cpp \
    ratio_tran.cpp \
    ratio_vdl.cpp \
    sqpacars.cpp \
    sqpvdl.cpp \
    start.cpp \
    vdl2.cpp \
    vesion.cpp

HEADERS += \
    acceuil.h \
    ratio_tran.h \
    ratio_vdl.h \
    sqpacars.h \
    sqpvdl.h \
    start.h \
    vdl2.h \
    vesion.h

FORMS += \
    acceuil.ui \
    ratio_tran.ui \
    ratio_vdl.ui \
    sqpacars.ui \
    sqpvdl.ui \
    start.ui \
    vdl2.ui \
    vesion.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
