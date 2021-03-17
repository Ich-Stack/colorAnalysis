QT -= gui
QT += core

CONFIG += c++11

CONFIG+=debug_and_release
CONFIG(debug, debug|release){
    TARGET = demo_debug
} else {
    TARGET = demo_release
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    posix_qextserialport.cpp \
    qextserialbase.cpp \
    camera.cpp \
    MatViewer.cpp \
    TYThread.cpp \
    main.cpp

INCLUDEPATH += \
    $$PWD/include \
    /usr/local/include \
    /usr/local/include/opencv4 \
    /usr/local/include/opencv4/opencv2 \

DEPENDPATH += \
    $$PWD/include \
    /usr/local/include \
    /usr/local/include/opencv4 \
    /usr/local/include/opencv4/opencv2 \

HEADERS += \
    posix_qextserialport.h \
    qextserialbase.h \
    camera.h

unix:!macx: LIBS += -L$$PWD/lib/ -ltycam
LIBS += `pkg-config --cflags --libs opencv`
