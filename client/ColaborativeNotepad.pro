QT       += core gui
QT       += network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    new.cpp \
    note.cpp \
    otdocument.cpp

HEADERS += \
    mainwindow.h \
    new.h \
    note.h \
    otdocument.h

FORMS += \
    mainwindow.ui \
    new.ui \
    note.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/alexandra/Desktop/ColaborativeNotead/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
