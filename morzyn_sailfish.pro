# The name of your app
TARGET = harbour-morzyn

CONFIG += sailfishapp

QT += xml

DEFINES += SAILFISH

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
	qml/* \
	qml/cover/* \
	qml/database/* \
	qml/fonts/* \
	qml/images/* \
	qml/sounds/* \
        translations/* \
    qml/Views.qml \
    qml/TitlePage.qml \
    qml/MorzynPage.qml \
    harbour-morzyn.desktop
