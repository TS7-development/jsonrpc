CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \

HEADERS += \
    util/asjson.hpp \
    util/fromjson.hpp \
    util/always_false.hpp \
    util/remove_cref.hpp \
    util/jsontype.hpp \
    util/jsonstreamer.hpp \
    util/fsm.hpp \
    util/util.hpp \
    error/errorcodes.hpp \
    error/maybe_failed.hpp \
    error/error.hpp \
    com/tcpserver.hpp \
    com/tcpconnection.hpp \
    parameter.hpp \
    error.hpp \
    notification.hpp \
    request.hpp \
    response.hpp \
    error_handler.hpp \
    notification_handler.hpp \
    request_handler.hpp \
    response_handler.hpp \
    procedure.hpp \
    module.hpp \
    jsonrpc.hpp

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
