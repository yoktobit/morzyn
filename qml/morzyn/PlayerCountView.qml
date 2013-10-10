// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    property string type
    property int max
    property int min
    property int selected
    signal changed()
    anchors.fill: parent
    Image {
        id: playerCountViewBack
        source: "images/titleback.png"
        anchors.fill: parent
    }
    Rectangle {
        id: playerCountViewShade
        color: "gray"
        opacity: 0.5
        anchors.fill: parent
    }
    Text {
        font.family: "VivaldiD"
        font.pixelSize: (50 * mainWindow.height) / mainWindow.sourceHeight
        text: type == "total" ? qsTr("How many total players?", "How many total players question") : qsTr("How many human players?", "How many human players question")
        anchors.top: playerCountViewShade.top
        anchors.horizontalCenter: playerCountViewShade.horizontalCenter
        anchors.topMargin: playerCountViewShade.height / 5
    }
    Row {
        spacing: 20
        Repeater {
            model: 8
            delegate: Text {
                font.family: "VivaldiD"
                font.pixelSize: (70 * mainWindow.height) / mainWindow.sourceHeight
                text: index
                visible: (index <= max) && (index >= min)
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton
                    propagateComposedEvents: true
                    onClicked: {
                        console.debug("Selected " + index);
                        selected = index;
                        changed();
                        game.state = (game.state === "totalPlayerCountState") ? "humanPlayerCountState" : "playerNameState"
                    }
                    onEntered: {
                        parent.color = "red";
                    }
                    onExited: {
                        parent.color = "black";
                    }
                }
            }
        }
        anchors.centerIn: parent
    }
}
