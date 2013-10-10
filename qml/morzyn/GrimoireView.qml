// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
import de.yoktobit.software.morzyn 1.0

Rectangle {
    id: grimoireView
    anchors.fill: parent

    color: "blue"
    property var creature1: null
    property var creature2: null
    /*
    Image {
        id: playerCountViewBack
        source: "images/titleback.png"
        anchors.fill: parent
    }
    ListView {
        id: grimoireLeftList
        visible: creature1 === null
        x: parent.width * 0.2
        y: parent.height * 0.1
        width: parent.width * 0.3
        height: parent.height * 0.6
        model: library.creatures
        delegate: GrowingText {
            standardSize: 18
            text: modelData.species
            color: "green"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    creature1 = modelData;
                    console.log(modelData.species);
                    console.log(modelData.unitClass);
                }
            }
        }
    }
    ListView {
        id: grimoireRightList
        visible: creature2 === null
        x: parent.width * 0.6
        y: parent.height * 0.1
        width: parent.width * 0.3
        height: parent.height * 0.6
        model: library.creatures
        delegate: GrowingText {
            standardSize: 18
            text: modelData.species
            color: "green"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    creature2 = modelData;
                }
            }
        }
    }
    Item {
        id: grimoireLeftDetails
        visible: creature1 !== null
        x: parent.width * 0.2
        y: parent.height * 0.1
        width: parent.width * 0.3
        height: parent.height * 0.6
        Column {
            anchors.fill: parent
            spacing: 10 * mainWindow.height / mainWindow.sourceHeight
            GrowingText {
                standardSize: 20
                text: creature1 ? creature1.species : ""
            }
            CreatureInformation {
                hoveredCreature: creature1
                player: Player {
                    spellPoints: 10000
                }
                positiveColor: "black"
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                gameService.message = "";
                creature1 = null;
            }
        }
    }

    Item {
        id: grimoireRightDetails
        x: parent.width * 0.6
        y: parent.height * 0.1
        width: parent.width * 0.3
        height: parent.height * 0.6
        visible: creature2 !== null
        Column {
            anchors.fill: parent
            spacing: 10 * mainWindow.height / mainWindow.sourceHeight
            GrowingText {
                standardSize: 20
                text: creature2 ? creature2.species : ""
            }
            CreatureInformation {
                hoveredCreature: creature2
                player: Player {
                    spellPoints: 10000
                }
                positiveColor: "black"
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                gameService.message = "";
                creature2 = null;
            }
        }
    }
    GrowingText {
        text: gameService.message
        standardSize: 24
        color: "red"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 140 * mainWindow.height / mainWindow.sourceHeight
    }

    MorzynButton {
        text: qsTr("Fight!", "Fight in Grimoire")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20 * mainWindow.height / mainWindow.sourceHeight
        visible: creature1 && creature2
        onClicked: {
            console.log("Fight " + creature1.species + " against " + creature2.species);
            gameService.simulateFight(creature1, creature2);
        }
    }
    MorzynButton {
        text: qsTr("Back", "Back in Grimoire")
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20 * mainWindow.height / mainWindow.sourceHeight
        onClicked: {
            game.state = "mainMenuState";
        }
    }*/
}
