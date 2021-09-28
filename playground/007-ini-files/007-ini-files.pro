TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        01-simple.cpp \
        02-structured.cpp \
        03-structured-variant.cpp \
        main.cpp

LIBS += -static -lboost_filesystem

HEADERS += \
    ini-files.h
