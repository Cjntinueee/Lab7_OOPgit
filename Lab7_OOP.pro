QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Arrow.cpp \
    ConcrFactory.cpp \
    Ellipse.cpp \
    Figures.cpp \
    Group.cpp \
    MyStorage.cpp \
    Object.cpp \
    Rectangle.cpp \
    TreeView.cpp \
    Triangle.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Arrow.h \
    ConcrFactory.h \
    Ellipse.h \
    Factory.h \
    Figures.h \
    Group.h \
    MyStorage.h \
    Object.h \
    Observer.h \
    Rectangle.h \
    TreeView.h \
    Triangle.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
