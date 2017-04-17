# Qt project file - qmake uses his to generate a Makefile

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QwtExample

LIBS += -lqwt -lm -lfftw3 -lbcm2835

HEADERS += window.h specanal.h ADCreader.h gpio-sysfs.h gz_clk.h bcm2835.h
SOURCES += main.cpp window.cpp specanal.cpp ADCreader.cpp gpio-sysfs.cpp gz_clk.cpp bcm2835.c
