# The name of your app
TARGET = harbour-morzyn

CONFIG += sailfishapp

#QT += quick qml xml
QT += xml

DEFINES += SAILFISH

#CONFIG += link_pkgconfig
#PKGCONFIG += sailfishapp
#INCLUDEPATH += /usr/include/sailfishapp

#TARGETPATH = /usr/bin
#target.path = $$TARGETPATH

#DEPLOYMENT_PATH = /usr/share/$$TARGET
#qml.files = qml
#qml.path = $$DEPLOYMENT_PATH

#desktop.files = harbour-morzyn.desktop
#desktop.path = /usr/share/applications

#icon.files = harbour-morzyn.png
#icon.path = /usr/share/icons/hicolor/86x86/apps

#INSTALLS += target icon desktop qml

# C++ sources
SOURCES += cpp/morzyn.cpp \
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
    cpp/dbloader.cpp \
    cpp/tutorialopponentai.cpp \
    cpp/tutorialplayerai.cpp

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
    cpp/dbloader.h \
    cpp/tutorialopponentai.h \
    cpp/tutorialplayerai.h

OTHER_FILES = \
    rpm/harbour-morzyn.yaml \
    rpm/harbour-morzyn.spec \
    qml/*.* \
    harbour-morzyn.desktop
