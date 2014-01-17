// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {

    signal exitTitle

    Image {
        source: "images/titelbild.png"
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onClicked: {
                titleViewTimer.stop();
                //game.state = "mainMenuState";
                //exitTitle();
            }
        }
        Image {
            source: "images/menu text bat.png"
            anchors.centerIn: parent
            width: height * sourceSize.width / sourceSize.height
            height: ((sourceSize.height * mainWindow.myHeight) / 600)
        }
    }
    Timer {
        id: titleViewTimer
        interval: 5000
        repeat: false
        running: true
        onTriggered: {
            //game.state = "mainMenuState";
            //exitTitle();
        }
    }
    onVisibleChanged: {
        if (visible)
        {
            if (os !== "sailfish")
                titleSound.volume = 1.0;
            else
                titleSound.play();
        }
    }
    anchors.fill: parent
}
