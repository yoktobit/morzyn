# Add more folders to ship with the application, here
folder_01.source = qml
folder_01.target = .
folder_02.source = translations
folder_02.target = .
DEPLOYMENTFOLDERS = folder_01 folder_02

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

#CONFIG += console

TARGET = morzyn

QT += xml multimedia


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

lupdate_only {
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
}

RC_FILE += morzyn.rc

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

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

OTHER_FILES += \
    ts.list \
    android/AndroidManifest.xml \
    morzyn.rc \
    android/res/layout/splash.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/version.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
