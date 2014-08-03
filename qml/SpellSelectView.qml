// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    anchors.fill: parent
    color: "transparent"

    property var hoverCreature: null
    property variant selectedCreature

    Row {
        Item {
            width: mainWindow.myWidth * 5/7
            height: mainWindow.myHeight
            Image {
                id: wiese
                source: "images/wiese.png"
                opacity: 0.1
                anchors.fill: parent
            }
            MenuButton {
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 0.02 * mainWindow.myWidth
            }
        }
        Image {
            id: statusback
            source: "images/statusback.png"
            visible: true

            width: mainWindow.myWidth * 2/7
            height: mainWindow.myHeight

            BackButton {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: (80 * mainWindow.myHeight) / mainWindow.sourceHeight
                anchors.horizontalCenter: parent.horizontalCenter
                onBackClicked: {
                    if (checkInput()) return;
                    if (os !== "android")
                        gameService.abort();
                    else
                    {
                        if (hoverCreature)
                        {
                            game.tempCreature = hoverCreature;
                            gameService.tryBuy(hoverCreature);
                            selectedCreature = hoverCreature;
                        }
                        else
                            gameService.abort();
                    }
                }
            }

            Column {
                anchors.topMargin: (80 * mainWindow.myHeight) / mainWindow.sourceHeight
                anchors.bottomMargin: (80 * mainWindow.myHeight) / mainWindow.sourceHeight
                anchors.leftMargin: (30 * mainWindow.myWidth) / mainWindow.sourceWidth
                anchors.rightMargin: (30 * mainWindow.myWidth) / mainWindow.sourceWidth
                anchors.fill: parent
                visible: hoverCreature !== null
                Text {
                    font.pixelSize: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
                    text: hoverCreature ? hoverCreature.species : ""
                    color: (!hoverCreature || hoverCreature.manaCost == "undefined") ? "#000000" : (game.currentPlayer.spellPoints >= hoverCreature.manaCost ? "#FFFFFF" : "#AAAAAA")
                }
                Item {
                    height: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
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


            }
            GrowingText {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: (145 * mainWindow.myHeight) / mainWindow.sourceHeight
                //color: "#1111FF"
                color: "#FFFFFF"
                standardSize: 15
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Available mana", "available mana in SpellSelectView")
            }

            Text {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: (105 * mainWindow.myHeight) / mainWindow.sourceHeight
                height: (50 * mainWindow.myHeight) / mainWindow.sourceHeight
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
                text: game.currentPlayer ? game.currentPlayer.spellPoints : ""
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#7777FF"
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
            font.pixelSize: (35 * mainWindow.myHeight) / mainWindow.sourceHeight
            font.family: "VivaldiD"
            color: "white"
        }
    }
    Component {
        id: spellSelectComponent
        Rectangle {
            id: spellSelectComponentRectangle
            width: Math.max(spellSelectComponentText.width, (230 * mainWindow.myWidth) / mainWindow.sourceWidth)
            height: spellSelectComponentText.height
            color: "transparent"
            //border.width: 4
            //border.color: hoverCreature === modelData ? "red" : "transparent"
            Text {
                id: spellSelectComponentText
                text: species
                //width: 230
                font.pixelSize: (34 * mainWindow.myHeight) / mainWindow.sourceHeight
                font.family: "VivaldiD"
                color: modelData && game.currentPlayer.spellPoints >= manaCost ? (hoverCreature === modelData ? "#00FF00" : "#FFFFFF") : (hoverCreature === modelData ? "red" : "#AAAAAA")
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton
                    //propagateComposedEvents: true
                    onEntered: {
                        if (os !== "android")
                            hoverCreature = modelData;
                    }
                    onExited: {
                    }
                    onClicked: {
                        if (checkInput()) return;
                        if (os !== "android")
                        {
                            game.tempCreature = modelData;
                            gameService.tryBuy(modelData);
                            selectedCreature = modelData;
                        }
                        else
                        {
                            if (hoverCreature === modelData)
                                hoverCreature = null;
                            else
                                hoverCreature = modelData;
                        }
                    }
                }
            }
        }
    }

    Flow {
        x: 20
        y: (40 * mainWindow.myHeight) / mainWindow.sourceHeight
        flow: GridView.TopToBottom
        width: wiese.width
        height: wiese.height - ((40 * mainWindow.myHeight) / mainWindow.sourceHeight)
        spacing: 5
        Repeater {
            model: game.currentPlayer ? game.currentPlayer.possibleCreatures : null
            delegate: spellSelectComponent
        }
    }
}
