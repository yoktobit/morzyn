// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {

    signal exitTitle

    property Loader tempLoader

    Image {
        source: "images/titelbild.png"
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onClicked: {
                titleViewTimer.stop();
                if (tempLoader.status === Loader.Ready)
                {
                    game.state = "mainMenuState";
                }
                //exitTitle();
            }
        }
        Image {
            source: "images/menu text bat.png"
            anchors.centerIn: parent
            width: height * sourceSize.width / sourceSize.height
            height: ((sourceSize.height * mainWindow.myHeight) / 600)
        }
        Rectangle {
            width: childrenRect.width
            height: childrenRect.height
            color: "#AA222222"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0.1 * parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            GrowingText {
                standardSize: 20
                color: "white"
                text: tempLoader.status === Loader.Ready ? qsTr("Press any key to continue...") : qsTr("Loading...")
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
    }
    Connections {
        target: tempLoader
        onStatusChanged: {
            if (tempLoader.status === Loader.Ready)
            {
                titleViewTimer.start();
            }
        }
    }
    Connections {
        target: game
        onStateChanged: {
            if (game.state == "spellSelectState")
            {
                /*if (os === "sailfish_sick")
                    titleSound.pause();
                else
                    titleSound.volume = 0.0;*/
                gameService.playTitleSong(false);
            }
        }
    }

    Timer {
        id: titleViewTimer
        interval: 5000
        repeat: false
        running: false
        onTriggered: {
            game.state = "mainMenuState";
            //exitTitle();
        }
    }
    anchors.fill: parent
    focus: true
    Keys.enabled: true
    Keys.onPressed: {
        if (!visible) return;
        console.debug("TitleView KEY_PRESSED");
        titleViewTimer.stop();
        if (tempLoader.status === Loader.Ready)
        {
            event.accepted = true;
            game.state = "mainMenuState";
        }
    }
}
