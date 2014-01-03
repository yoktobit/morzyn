// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
//import QtQuick.Controls 1.0

Rectangle {
    anchors.fill: parent
    color: "transparent"

    property var hoverCreature: null
    property variant selectedCreature

    Row {
        Item {
            width: mainWindow.width * 5/7
            height: mainWindow.height
            Image {
                id: wiese
                source: "images/wiese.png"
                opacity: 0.1
                anchors.fill: parent
            }
            MenuButton {
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 0.02 * mainWindow.width
            }
        }
        Image {
            id: statusback
            source: "images/statusback.png"
            visible: true

            width: mainWindow.width * 2/7
            height: mainWindow.height

            BackButton {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: (80 * mainWindow.height) / mainWindow.sourceHeight
                anchors.horizontalCenter: parent.horizontalCenter
                onBackClicked: {
                    if (checkInput()) return;
                    gameService.abort();
                }
            }

            Column {
                anchors.topMargin: (80 * mainWindow.height) / mainWindow.sourceHeight
                anchors.bottomMargin: (80 * mainWindow.height) / mainWindow.sourceHeight
                anchors.leftMargin: (30 * mainWindow.width) / mainWindow.sourceWidth
                anchors.rightMargin: (30 * mainWindow.width) / mainWindow.sourceWidth
                anchors.fill: parent
                visible: hoverCreature !== null
                Text {
                    font.pixelSize: (20 * mainWindow.height) / mainWindow.sourceHeight
                    text: hoverCreature ? hoverCreature.species : ""
                    color: (!hoverCreature || hoverCreature.manaCost == "undefined") ? "#000000" : (game.currentPlayer.spellPoints >= hoverCreature.manaCost ? "#00FF00" : "red")
                }
                Item {
                    height: (20 * mainWindow.height) / mainWindow.sourceHeight
                    width: 100
                }
                CreatureInformation {
                    hoveredCreature: hoverCreature
                    player: game.currentPlayer
                }
                ScrollInformation {
                    hoveredCreature: hoverCreature
                    player: game.currentPlayer
                }

                Text {
                    height: (50 * mainWindow.height) / mainWindow.sourceHeight
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: (20 * mainWindow.height) / mainWindow.sourceHeight
                    text: game.currentPlayer ? game.currentPlayer.spellPoints : ""
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: (!hoverCreature || hoverCreature.manaCost == "undefined") ? "#000000" : (game.currentPlayer.spellPoints >= hoverCreature.manaCost ? "#00FF00" : "red")
                }
            }
        }
    }

    Rectangle {
        width: wiese.width
        height: wiese.height
        color: "gray"
        opacity: 0.5

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: game.currentPlayer ? game.currentPlayer.name : ""
            font.pixelSize: (35 * mainWindow.height) / mainWindow.sourceHeight
            font.family: "VivaldiD"
            color: "white"
        }
    }
    Component {
        id: spellSelectComponent
        Item {
            width: Math.max(spellSelectComponentText.width, (230 * mainWindow.width) / mainWindow.sourceWidth)
            height: spellSelectComponentText.height
            Text {
                id: spellSelectComponentText
                text: species
                //width: 230
                font.pixelSize: (34 * mainWindow.height) / mainWindow.sourceHeight
                font.family: "VivaldiD"
                color: game.currentPlayer.spellPoints >= manaCost ? "#00FF00" : "red"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton
                    //propagateComposedEvents: true
                    onEntered: {
                        //parent.color = "red";
                        hoverCreature = modelData;
                    }
                    onExited: {
                        //parent.color = "black";
                    }
                    onClicked: {
                        if (checkInput()) return;
                        gameService.tryBuy(modelData);
                        selectedCreature = modelData;
                    }
                }
            }
        }
    }

    Flow {
        x: 20
        y: (40 * mainWindow.height) / mainWindow.sourceHeight
        flow: GridView.TopToBottom
        width: wiese.width
        height: wiese.height - ((40 * mainWindow.height) / mainWindow.sourceHeight)
        spacing: 5
        Repeater {
            model: game.currentPlayer ? game.currentPlayer.possibleCreatures : null
            delegate: spellSelectComponent
        }
    }
}
