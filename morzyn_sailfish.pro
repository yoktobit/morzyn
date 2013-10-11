# The name of your app
TARGET = morzyn

# C++ sources
SOURCES += cpp/sailfish/main.cpp \
    cpp/statistics.cpp \
    cpp/scroll.cpp \
    cpp/playercolor.cpp \
    cpp/player.cpp \
    cpp/iai.cpp \
    cpp/gameservice.cpp \
    cpp/game.cpp \
    cpp/enemyname.cpp \
    cpp/easyai.cpp \
    cpp/creature.cpp \
    cpp/dbloader.cpp

# C++ headers
HEADERS += \
    cpp/statistics.h \
    cpp/scroll.h \
    cpp/playercolor.h \
    cpp/player.h \
    cpp/iai.h \
    cpp/gameservice.h \
    cpp/game.h \
    cpp/enemyname.h \
    cpp/easyai.h \
    cpp/creature.h \
    cpp/constants.h \
    cpp/dbloader.h

# QML files and folders
qml.files = qml sailfish/qml

# The .desktop file
desktop.files = morzyn.desktop

# Please do not modify the following line.
include(cpp/sailfish/sailfishapplication/sailfishapplication.pri)

OTHER_FILES = \
    rpm/morzyn.yaml \
    rpm/morzyn.spec
