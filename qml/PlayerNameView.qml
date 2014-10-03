// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
import harbour.morzyn 1.0

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
            spacing: (10 * mainWindow.myHeight) / mainWindow.sourceHeight
            height: playerColorSelector.height
            z: game.players.length - index

            Text {
                text: originalName
                font.pixelSize: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
                color: "red"
            }
            Rectangle {
                width: (200 * mainWindow.myWidth) / mainWindow.sourceWidth
                height: (30 * mainWindow.myHeight) / mainWindow.sourceHeight
                border.width: 1
                border.color: "gray"
                color: "#55AAAAAA"
                TextInput {
                //TextField {
                    id: creatureNameComponentTextInput
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: (10 * mainWindow.myWidth) / mainWindow.sourceWidth
                    anchors.rightMargin: (10 * mainWindow.myWidth) / mainWindow.sourceWidth
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
                    text: name
                    mouseSelectionMode: TextInput.SelectCharacters
                    selectByMouse: true
                    maximumLength: 14
                    onTextChanged: {
                        if (index >= 0 && index < game.players.length)
                            game.players[index].name = text;
                        if (index >= 0 && index < game.creatures.length)
                            game.creatures[index].name = text;
                    }
                    focus: false
                    onActiveFocusChanged: {
                        if (activeFocus)
                        {
                            console.debug("active focus " + name);
                            selectAll();
                        }
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
                selectedValue: race
                model: ListModel {
                    ListElement {
                        name: "rogue"
                        value: "rogue"
                    }
                    ListElement {
                        name: "sorcerer"
                        value: "sorcerer"
                    }
                    ListElement {
                        name: "warrior"
                        value: "warrior"
                    }
                }
                onSelectedValueChanged: {
                    console.log("Race: " + selectedValue);
                    if (index >= 0 && index < game.players.length)
                        game.players[index].race = selectedValue;
                    if (index >= 0 && index < game.creatures.length)
                        game.creatures[index].race = selectedValue;
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
        anchors.fill: parent
        anchors.margins: (10 * mainWindow.myHeight) / mainWindow.sourceHeight
        focus: true
        delegate: creatureNameComponent
        model: game.players
        spacing: (10 * mainWindow.myHeight) / mainWindow.sourceHeight
        section.property: "type"
        section.delegate: Text {
            font.pixelSize: (30 * mainWindow.myHeight) / mainWindow.sourceHeight
            text: qsTr("%1Players", "for player category caption").arg(section)
            height: font.pixelSize + ((10 * mainWindow.myHeight) / mainWindow.sourceHeight)
            color: "red"
        }
    }
    GrowingText {
        id: warningText
        text: ""
        standardSize: 30
        color: "red"
        anchors.verticalCenter: goButton.verticalCenter
        anchors.left: parent.left
        anchors.margins: 10
    }

    MorzynButton {
        id: goButton
        text: "Go"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
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
    MenuButton {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 0.02 * mainWindow.myWidth
    }
}
