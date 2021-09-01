TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../

SOURCES += \
        main.cpp

LIBS += -lboost_json
