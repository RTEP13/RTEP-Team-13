# Qt project file - qmake uses his to generate a Makefile

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QwtExample

LIBS += -lqwt -lm -lfftw3

HEADERS += window.h specanal.h

SOURCES += main.cpp window.cpp specanal.cpp
