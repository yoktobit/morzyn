// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Image {
    id: morzynButton
    property alias text: morzynButtonText.text

    signal clicked

    source: "images/button.png"
    width: (sourceSize.width * mainWindow.width) / 800 // fix, weil sonst zu groß
    height: (sourceSize.height * mainWindow.height) / 600 // fix, weil sonst zu groß

    Text {
        id: morzynButtonText
        font.pixelSize: (30 * mainWindow.height) / mainWindow.sourceHeight
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
