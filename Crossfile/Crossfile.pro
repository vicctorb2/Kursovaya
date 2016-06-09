
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Crossfile
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Pane.cpp \
    PreferencesDialog.cpp \
    Properties.cpp

HEADERS  += MainWindow.h \
    Pane.h \
    PreferencesDialog.h \
    Properties.h

RESOURCES += \
    Resources.qrc











