# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = FIR
DESTDIR = ../debug
QT += core network widgets gui
CONFIG += debug
DEFINES += _WINDOWS QT_WIDGETS_LIB QT_NETWORK_LIB
INCLUDEPATH += . \
    ./debug \
    . \
    $(QTDIR)/mkspecs/win32-msvc2010 \
    ./GeneratedFiles
LIBS += -L"../../../../qt/5.5/msvc2010/lib" \
    -lshell32
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(FIR.pri)
