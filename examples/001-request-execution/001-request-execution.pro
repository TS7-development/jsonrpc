TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../

LIBS += -static -lboost_json

SOURCES += \
        main.cpp
