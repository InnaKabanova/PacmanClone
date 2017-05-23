TEMPLATE = app

QT += qml quick
CONFIG += c++11
CONFIG += c++14

SOURCES += main.cpp \
    currentgame.cpp \
    pacman.cpp \
    ghostmanager.cpp \
    clyde.cpp \
    roommanager.cpp \
    pinky.cpp \
    inky.cpp \
    blinky.cpp \
    ghost.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

HEADERS += \
    currentgame.hpp \
    ghost.hpp \
    ghostmanager.hpp \
    pacman.hpp \
    clyde.hpp \
    roommanager.hpp \
    pinky.hpp \
    inky.hpp \
    blinky.hpp \
    direction.hpp \
    roomtypes.hpp \
    globalconfig.hpp \
    position.hpp

DISTFILES +=
