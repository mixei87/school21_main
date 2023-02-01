QT += core gui opengl openglwidgets

include(QtGifImage/src/gifimage/qtgifimage.pri)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = viewer
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    colorform.cpp \
    main.cpp \
    mainwindow.cpp \
    parser.c \
    s21_matrix.c \
    settingform.cpp \
    widget.cpp

HEADERS += \
    colorform.h \
    mainwindow.h \
    parser.h \
    s21_error.h \
    s21_matrix.h \
    settingform.h \
    widget.h

FORMS += \
    colorform.ui \
    mainwindow.ui \
    settingform.ui

RESOURCES += \
    shaders.qrc \
    textures.qrc \

DISTFILES += \
  objects/*

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ICON = scull.icns
