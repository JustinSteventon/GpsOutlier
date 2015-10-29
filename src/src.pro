QT += core
QT -= gui

TARGET = src
CONFIG += console
CONFIG -= app_bundle

DEFINES += QTBUILD

QMAKE_CXXFLAGS += -Wno-unknown-pragmas
QMAKE_CXXFLAGS += -Wno-multichar
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-write-strings
QMAKE_CXXFLAGS += -Wno-sign-compare
QMAKE_CXXFLAGS += -Wno-narrowing
QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CXXFLAGS += -Wno-missing-field-initializers
QMAKE_CXXFLAGS += -Wno-type-limits
QMAKE_CXXFLAGS += -Wno-switch
QMAKE_CXXFLAGS += -Wno-parentheses
QMAKE_CXXFLAGS += -Wno-switch
QMAKE_CXXFLAGS += -Wno-reorder
QMAKE_CXXFLAGS += -Wno-switch
QMAKE_CXXFLAGS += -Wno-unused-but-set-variable
QMAKE_CXXFLAGS += -Wno-delete-non-virtual-dtor
QMAKE_CXXFLAGS += -Wno-sequence-point
QMAKE_CXXFLAGS += -Wno-unknown-pragmas
QMAKE_CXXFLAGS += -Wno-address
QMAKE_CXXFLAGS += -Wno-unused-function
QMAKE_CXXFLAGS += -Wno-unused-value

TEMPLATE = app

SOURCES += \
    main.cpp \
    fxNMEA.cpp \
    fxUtils.cpp \
    fxLists.cpp \
    fxMath.cpp \
    QtUtils.cpp \
    fxTypes.cpp

HEADERS += \
    fxGPSParse.h \
    fxTypes.h \
    fxNMEA.h \
    fxPlatform.h \
    fxUtils.h \
    fxLists.h \
    fxMath.h \
    QtUtils.h

