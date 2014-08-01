TEMPLATE = app

TARGET = morzyn

QT += qml quick widgets xml multimedia

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += cpp/desktopandroid/main.cpp \
    cpp/dbloader.cpp \
    cpp/iai.cpp \
    cpp/gameservice.cpp \
    cpp/game.cpp \
    cpp/enemyname.cpp \
    cpp/easyai.cpp \
    cpp/creature.cpp \
    cpp/scroll.cpp \
    cpp/playercolor.cpp \
    cpp/player.cpp \
    cpp/statistics.cpp \
    cpp/tutorialplayerai.cpp \
    cpp/tutorialopponentai.cpp

TRANSLATIONS = translations/morzyn_de.ts

OTHER_FILES += qml/BackButton.qml \
    qml/CreatureImage.qml \
    qml/CreatureInformation.qml \
    qml/DistanceAttackImage.qml \
    qml/DropList.qml \
    qml/GameOverView.qml \
    qml/GameView.qml \
    qml/GrimoireView.qml \
    qml/GrowingText.qml \
    qml/InGameMenuView.qml \
    qml/main.qml \
    qml/MainMenuView.qml \
    qml/MenuButton.qml \
    qml/morzyn.qml \
    qml/MorzynAudio.qml \
    qml/MorzynCheckBox.qml \
    qml/MorzynPage.qml \
    qml/OptionsView.qml \
    qml/Views.qml \
    qml/MorzynButton.qml \
    qml/PlayerColorSelector.qml \
    qml/PlayerCountView.qml \
    qml/PlayerNameView.qml \
    qml/Scroll.qml \
    qml/ScrollInformation.qml \
    qml/SpellSelectView.qml \
    qml/Switch.qml \
    qml/TitleView.qml \
    qml/TitlePage.qml

RC_FILE += morzyn.rc

HEADERS += \
    cpp/dbloader.h \
    cpp/constants.h \
    cpp/iai.h \
    cpp/gameservice.h \
    cpp/game.h \
    cpp/enemyname.h \
    cpp/easyai.h \
    cpp/creature.h \
    cpp/playercolor.h \
    cpp/player.h \
    cpp/scroll.h \
    cpp/statistics.h \
    cpp/tutorialplayerai.h \
    cpp/tutorialopponentai.h

#OTHER_FILES += \
#    ts.list \
#    android/AndroidManifest.xml \
#    morzyn.rc \
#    android/res/layout/splash.xml \
#    android/res/values/strings.xml \
#    android/res/values-de/strings.xml \
#    android/res/values-el/strings.xml \
#    android/res/values-es/strings.xml \
#    android/res/values-et/strings.xml \
#    android/res/values-fa/strings.xml \
#    android/res/values-fr/strings.xml \
#    android/res/values-id/strings.xml \
#    android/res/values-it/strings.xml \
#    android/res/values-ja/strings.xml \
#    android/res/values-ms/strings.xml \
#    android/res/values-nb/strings.xml \
#    android/res/values-nl/strings.xml \
#    android/res/values-pl/strings.xml \
#    android/res/values-pt-rBR/strings.xml \
#    android/res/values-ro/strings.xml \
#    android/res/values-rs/strings.xml \
#    android/res/values-ru/strings.xml \
#    android/res/values-zh-rCN/strings.xml \
#    android/res/values-zh-rTW/strings.xml \
#    android/version.xml \
#    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
#    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
#    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
#    android/src/org/qtproject/qt5/android/bindings/QtApplication.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

android {
    ASSET_INSTALL_PATH = /assets
    folder_01.files = qml/*
    folder_01.path = $$ASSET_INSTALL_PATH/qml
    folder_01.depends += FORCE
    folder_02.files = translations/*
    folder_02.path = $$ASSET_INSTALL_PATH/translations
    folder_01.depends += FORCE
    INSTALLS += folder_01 folder_02
} else {
    ASSET_INSTALL_PATH = $$OUT_PWD
    folder_01.source = qml
    folder_01.path = .
    folder_02.source = translations
    folder_02.path = .
    DEPLOYMENTFOLDERS += folder_01 folder_02
}

# Default rules for deployment.
include(deployment.pri)
