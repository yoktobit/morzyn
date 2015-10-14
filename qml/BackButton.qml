// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    width: backButtonGrowingText.width * 2
    height: backButtonGrowingText.height
    color: "#55555555"
    border.width: 1
    border.color: "#888888"

    signal backClicked

    /*GrowingText {
        id: backButtonGrowingText
        color: "red"
        anchors.centerIn: parent
        standardSize: 20
        text: "<-----"
    }*/
    Image {
        id: backButtonGrowingText
        source: "images/haken.png"
        width: 36 * mainWindow1.myWidth / mainWindow.sourceWidth
        height: 36 * mainWindow1.myHeight / mainWindow.sourceHeight
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        propagateComposedEvents: true
        onClicked: {
            if (checkInput()) return;
            backClicked();
        }
    }
}
