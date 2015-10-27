QT += core
QT -= gui
  
QMAKE_MAC_SDK = macosx10.11
CONFIG += console c++11
CONFIG -= add_bundle
TEMPLATE = app
TARGET = gpsoutlier

SOURCES += main.cpp \
    Complex.cpp \
    Math.cpp \
    Outlier.cpp

HEADERS += Complex.hpp \
    Math.hpp \
    Util.hpp \
    Outlier.hpp

  
