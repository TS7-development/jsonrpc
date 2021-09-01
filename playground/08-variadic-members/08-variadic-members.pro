TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        01-dump.cpp \
        02-function-call.cpp \
        main.cpp

HEADERS += \
    variadic-members.h

LIBS += -lboost_json
