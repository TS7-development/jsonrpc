CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \

HEADERS += \
    asjson.hpp \
    callfromjson.hpp \
    error.hpp \
    errorcodes.hpp \
    fromjson.hpp \
    jsonrpc.hpp \
    notification.hpp \
    procedure.hpp \
    request.hpp \
    response.hpp \
    util.hpp

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
