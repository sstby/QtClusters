QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    data.cpp \
    graphicview.cpp \
    main.cpp \
    mainwindow.cpp \
    newdatadialog.cpp

HEADERS += \
    data.h \
    graphicview.h \
    mainwindow.h \
    newdatadialog.h

FORMS += \
    graphicview.ui \
    mainwindow.ui \
    newdatadialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    C:/Users/Klymenko Konstantin/Documents/build-clasters-Desktop_Qt_6_1_3_MinGW_64_bit-Debug/tmp.py \
    C:/Users/Klymenko Konstantin/Documents/build-clasters-Desktop_Qt_6_1_3_MinGW_64_bit-Debug/tmp.py \
    clasters_script.py \
    functions.py

