// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
import de.yoktobit.software.morzyn 1.0

Rectangle {
    id: playerNameView
    anchors.fill: parent
    Image {
        id: playerNameViewBack
        source: "images/titleback.png"
        anchors.fill: parent
    }
    Connections {
        target: humanPlayerCountView
        onChanged: {
            gameService.addPlayers(totalPlayerCountView.selected, humanPlayerCountView.selected);
        }
    }
    Component {
        id: creatureNameComponent
        Row {
            spacing: (10 * mainWindow.height) / mainWindow.sourceHeight
            height: playerColorSelector.height
            z: game.players.length - index

            Text {
                text: originalName
                font.pixelSize: (20 * mainWindow.height) / mainWindow.sourceHeight
                color: "red"
            }
            Rectangle {
                width: (200 * mainWindow.width) / mainWindow.sourceWidth
                height: (30 * mainWindow.height) / mainWindow.sourceHeight
                border.width: 1
                border.color: "gray"
                color: "#55AAAAAA"
                TextInput {
                //TextField {
                    id: creatureNameComponentTextInput
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: (10 * mainWindow.width) / mainWindow.sourceWidth
                    anchors.rightMargin: (10 * mainWindow.width) / mainWindow.sourceWidth
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: (20 * mainWindow.height) / mainWindow.sourceHeight
                    text: name
                    mouseSelectionMode: TextInput.SelectCharacters
                    selectByMouse: true
                    maximumLength: 14
                    onTextChanged: {
                        game.players[index].name = text;
                        game.creatures[index].name = text;
                    }
                    onActiveFocusChanged: {
                        if (activeFocus)
                            selectAll();
                    }
                }
            }

            PlayerColorSelector {
                id: playerColorSelector
                current: index
                onCurrentChanged: {
                    warningText.text = "";
                }
            }
            Switch {
                id: playerColorCategory
                selectedItem: race
                model: ListModel {
                    ListElement {
                        name: "rogue"
                    }
                    ListElement {
                        name: "sorcerer"
                    }
                    ListElement {
                        name: "warrior"
                    }
                }
                onSelectedItemChanged: {
                    console.log("Race: " + selectedItem);
                    game.players[index].race = selectedItem;
                    game.creatures[index].race = selectedItem;
                }
            }
            /*DropList {
                id: playerColorTeam
                selectedItem: index + 1
                model: ListModel {
                    ListElement {
                        name: "1"
                    }
                    ListElement {
                        name: "2"
                    }
                    ListElement {
                        name: "3"
                    }
                    ListElement {
                        name: "4"
                    }
                    ListElement {
                        name: "5"
                    }
                    ListElement {
                        name: "6"
                    }
                    ListElement {
                        name: "7"
                    }
                }
            }
            DropList {
                id: playerColorDifficulty
                selectedItem: "easy"
                model: ListModel {
                    ListElement {
                        name: "easy"
                    }
                    ListElement {
                        name: "hard"
                    }
                }
            }*/
        }
    }
    ListView {
        id: playerNameViewListView
        anchors.fill: playerNameViewBack
        anchors.margins: 20
        delegate: creatureNameComponent
        model: game.players
        spacing: (10 * mainWindow.height) / mainWindow.sourceHeight
        section.property: "type"
        section.delegate: Text {
            font.pixelSize: (30 * mainWindow.height) / mainWindow.sourceHeight
            text: qsTr("%1Players", "for player category caption").arg(section)
            height: font.pixelSize + 10
            color: "red"
        }
    }
    GrowingText {
        id: warningText
        text: ""
        standardSize: 30
        color: "red"
        anchors.verticalCenter: goButton.verticalCenter
        anchors.left: playerNameViewBack.left
        anchors.margins: 10
    }

    MorzynButton {
        id: goButton
        text: "Go"
        anchors.bottom: playerNameViewBack.bottom
        anchors.right: playerNameViewBack.right
        anchors.margins: 10
        onClicked: {
            if (gameService.checkDuplicates())
            {
                warningText.text = qsTr("Duplicate player colors found!", "message, that duplicate player colors were found");
                return;
            }
            else
            {
                warningText.text = "";
            }

            gameService.start();
        }
    }
}
