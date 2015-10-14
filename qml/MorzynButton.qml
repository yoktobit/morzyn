// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Image {
    id: morzynButton
    property alias text: morzynButtonText.text

    signal clicked

    source: "images/button.png"
    width: parent.width * 0.27//(sourceSize.width * mainWindow.myWidth) / 800 // fix, weil sonst zu groß
    height: parent.height / 8.0//(sourceSize.height * mainWindow1.myHeight) / 600 // fix, weil sonst zu groß

    Text {
        id: morzynButtonText
        font.pixelSize: morzynButton.height * 0.4//(30 * mainWindow1.myHeight) / mainWindow.sourceHeight
        font.family: "VivaldiD"
        anchors.centerIn: parent
    }
    MouseArea {
        id: morzynButtonMouseArea
        hoverEnabled: true
        onEntered: {
            morzynButtonText.color = "red";
        }
        onExited: {
            morzynButtonText.color = "black";
        }
        onClicked: {
            morzynButton.clicked();
        }

        anchors.fill: parent
    }
}
