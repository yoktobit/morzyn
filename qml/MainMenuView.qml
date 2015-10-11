// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    anchors.fill: parent
    Image {
        id: mainMenuViewBack
        source: "images/titelbild.png"
        anchors.fill: parent
    }
    Image {
        id: mainMenuBanner
        source: "images/banner.png"
        x: parent.width * 0.1
        y: parent.height * 0.1
        width: height * sourceSize.width / sourceSize.height
        height: parent.height * 0.5//(sourceSize.height * mainWindow.myHeight) / 600 // Absicht mit der 600, weil sonst zu groß
    }
    Image {
        source: "images/menu fledermaeuse.png"
        x: parent.width * 0.1
        y: parent.height * 0.65
        width: height * sourceSize.width / sourceSize.height
        height: parent.height * 0.25//(sourceSize.height * mainWindow.myHeight) / 600 // Absicht mit der 600
    }
    MorzynButton {
        id: mainMenuStartButton
        x: parent.width * 0.62
        y: parent.height * 0.1
        text: qsTr("Start", "Start in main menu")
        onClicked: {
            gameService.resetGame();
            game.state = "totalPlayerCountState";
        }
    }
    /*MorzynButton {
        id: mainMenuTutorialButton
        x: parent.width * 0.62
        y: parent.height * 0.1
        text: qsTr("Tutorial", "Tutorial in main menu")
        onClicked: {
            gameService.resetGame();
            game.state = "totalPlayerCountState";
        }
    }*/
    MorzynButton {
        id: mainMenuGrimoireButton
        x: parent.width * 0.62
        y: parent.height * 0.25
        text: qsTr("Grimoire", "Grimoire in main menu")
        onClicked: {
            game.state = "grimoireState";
        }
    }
    MorzynButton {
        id: mainMenuOptionsButton
        x: parent.width * 0.62
        y: parent.height * 0.4
        text: qsTr("Options", "Options in main menu")
        onClicked: {
            game.state = "optionsState";
        }
    }
    MorzynButton {
        id: mainMenuHelpButton
        x: parent.width * 0.62
        y: parent.height * 0.55
        text: qsTr("Online Help", "Online Help in main menu")
        onClicked: {
            console.log("Locale: " + locale);
            allViews.fullscreen = false;
            console.log(allViews.fullscreen);
            gameService.setFullScreen(allViews.fullscreen);
            if (locale === "de_DE")
            {
                Qt.openUrlExternally("http://yoktobit.de/morzyn/pageger/pagereadme.html");
            }
            else
            {
                Qt.openUrlExternally("http://yoktobit.de/morzyn/pageusa/pagereadme.html");
            }
        }
    }
    MorzynButton {
        id: mainMenuQuitButton
        x: parent.width * 0.62
        y: parent.height * 0.7
        text: qsTr("Quit", "Quit in main menu")
        onClicked: {
            gameService.quit();
        }
    }
    Rectangle {
        width: childrenRect.width
        height: childrenRect.height
        color: "#AA222222"
        anchors.right: parent.right
        anchors.rightMargin: 0.1 * parent.width
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0.1 * parent.height
        GrowingText {
            standardSize: 20
            color: "white"
            text: version
        }
    }
    onVisibleChanged: {
        if (visible)
        {
            if (mainWindow.musicActivated && mainWindow.applicationActive)
            {
                /*titleSound.morzynPlay();
                if (os !== "sailfish_sick" && titleSound.volume !== 1.0)
                    titleSound.volume = 1.0;
                titleSound.isBehaviourEnabled = true;*/
                gameService.playTitleSong(true);
            }
        }
    }
}
