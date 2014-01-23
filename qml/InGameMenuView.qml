// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    id: inGameMenuView
    anchors.fill: parent
    color: "transparent"
    visible: false
    property var hoveredCreature: null

    Row {
        Image {
            id: wiese
            source: "images/wiese.png"
            opacity: 0.1
            visible: true
            width: mainWindow.myWidth * 5/7
            height: mainWindow.myHeight
        }
        Image {
            id: statusback
            source: "images/statusback.png"
            visible: true
            width: mainWindow.myWidth * 2/7
            height: mainWindow.myHeight
        }
    }

    Rectangle {
        width: wiese.width
        height: wiese.height
        color: "#AA222222"
    }

    Item {
        width: wiese.width
        height: wiese.height
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            onClicked: {
                console.log("click is not allowed here");
                mouse.accepted = true;
            }
        }
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Menu"
            font.pixelSize: (35 * mainWindow.myHeight) / mainWindow.sourceHeight
            font.family: "VivaldiD"
            color: "white"
        }
        Flow {
            //x: 20
            //y: (40 * mainWindow.myHeight) / mainWindow.sourceHeight
            flow: GridView.TopToBottom
            spacing: 10
            anchors.centerIn: parent
            Item {
                id: continueButton
                width: Math.max(spellSelectComponentText1.width, (230 * mainWindow.myWidth) / mainWindow.sourceWidth)
                height: spellSelectComponentText1.height
                Text {
                    id: spellSelectComponentText1
                    text: qsTr("Continue", "Continue in in-game-menu")
                    font.pixelSize: (34 * mainWindow.myHeight) / mainWindow.sourceHeight
                    font.family: "VivaldiD"
                    color: "white"
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.LeftButton
                        propagateComposedEvents: true
                        onEntered: {
                            parent.color = "red";
                        }
                        onExited: {
                            parent.color = "white";
                        }
                        onClicked: {
                            inGameMenuView.visible = false;
                        }
                    }
                }
            }
            Item {
                id: exitButton
                width: Math.max(spellSelectComponentText2.width, (230 * mainWindow.myWidth) / mainWindow.sourceWidth)
                height: spellSelectComponentText2.height
                Text {
                    id: spellSelectComponentText2
                    text: qsTr("Back to Main Menu", "Back to Main Menu in in-game-menu")
                    font.pixelSize: (34 * mainWindow.myHeight) / mainWindow.sourceHeight
                    font.family: "VivaldiD"
                    color: "white"
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.LeftButton
                        propagateComposedEvents: true
                        onEntered: {
                            parent.color = "red";
                        }
                        onExited: {
                            parent.color = "white";
                        }
                        onClicked: {
                            gameService.resetGame();
                            inGameMenuView.visible = false;
                            game.state = "mainMenuState";
                        }
                    }
                }
            }
            Item {
                id: quitButton
                width: Math.max(spellSelectComponentText2.width, (230 * mainWindow.myWidth) / mainWindow.sourceWidth)
                height: spellSelectComponentText2.height
                Text {
                    id: spellSelectComponentText3
                    text: qsTr("Quit", "Quit in main menu")
                    font.pixelSize: (34 * mainWindow.myHeight) / mainWindow.sourceHeight
                    font.family: "VivaldiD"
                    color: "white"
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.LeftButton
                        propagateComposedEvents: true
                        onEntered: {
                            parent.color = "red";
                        }
                        onExited: {
                            parent.color = "white";
                        }
                        onClicked: {
                            gameService.quit();
                        }
                    }
                }
            }
        }
    }

}
