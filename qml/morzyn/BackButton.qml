// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    width: backButtonGrowingText.width
    height: backButtonGrowingText.height
    color: "transparent"

    signal backClicked

    GrowingText {
        id: backButtonGrowingText
        color: "red"
        anchors.centerIn: parent
        standardSize: 20
        text: "<-----"
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
