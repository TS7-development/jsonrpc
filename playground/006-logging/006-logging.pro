TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        01-trivial.cpp \
        02-trivial-filter.cpp \
        03-sinks.cpp \
        04-scopes.cpp \
        main.cpp

HEADERS += \
    logging.h

LIBS += -static -lboost_log_setup -lboost_log -lboost_system -lboost_thread -lboost_filesystem -lpthread
