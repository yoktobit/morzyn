// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
import harbour.morzyn 1.0

Rectangle {
    id: grimoireView
    anchors.fill: parent

    property var creature1: null
    property var creature2: null

    Image {
        id: playerCountViewBack
        source: "images/titleback.png"
        anchors.fill: parent
    }
    Rectangle {
        color: "#AA222222"
        visible: creature1 === null
        x: parent.width * 0.1
        y: parent.height * 0.1
        width: parent.width * 0.3
        height: parent.height * 0.6
        ListView {
            id: grimoireLeftList
            anchors.fill: parent
            anchors.margins: (18 * mainWindow1.myHeight) / mainWindow.sourceHeight
            model: library.creatures
            delegate: GrowingText {
                standardSize: 18
                text: modelData.species
                color: "white"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        creature1 = modelData;
                        grimoireLeftImages.model = gameService.getCreatureImages(creature1.imageFilePattern);
                        console.log(modelData.species);
                        console.log(modelData.unitClass);
                    }
                }
            }
        }
    }
    GrowingText {
        text: "VS"
        standardSize: 30
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height * 0.3
    }

    Rectangle {
        color: "#AA222222"
        visible: creature2 === null
        x: parent.width * 0.6
        y: parent.height * 0.1
        width: parent.width * 0.3
        height: parent.height * 0.6
        ListView {
            id: grimoireRightList
            anchors.fill: parent
            anchors.margins: (18 * mainWindow1.myHeight) / mainWindow.sourceHeight
            model: library.creatures
            delegate: GrowingText {
                standardSize: 18
                text: modelData.species
                color: "white"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        creature2 = modelData;
                        grimoireRightImages.model = gameService.getCreatureImages(creature2.imageFilePattern);
                    }
                }
            }
        }
    }
    Rectangle {
        id: grimoireLeftDetails
        visible: creature1 !== null
        x: parent.width * 0.1
        y: parent.height * 0.1
        width: parent.width * 0.3
        height: parent.height * 0.6
        color: "#AA222222"
        Column {
            anchors.centerIn: parent
            spacing: 10 * mainWindow.height / mainWindow.sourceHeight
            GrowingText {
                color: "white"
                standardSize: 20
                text: creature1 ? creature1.species : ""
            }
            ListView {
                id: grimoireLeftImages
                orientation: ListView.Horizontal
                width: childrenRect.width
                height: 30 * mainWindow.height / mainWindow.sourceHeight
                delegate: Image {
                    id: creatureLeftImage
                    source: modelData === undefined || modelData === null ? "" : "images/" + modelData
                    height: sourceSize.height * mainWindow.height / mainWindow.sourceHeight // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
                    width: sourceSize.height * mainWindow.height / mainWindow.sourceHeight
                }
            }
            CreatureInformation {
                hoveredCreature: creature1
                player: Player {
                    spellPoints: 10000
                }
                positiveColor: "white"
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

    Rectangle {
        id: grimoireRightDetails
        x: parent.width * 0.6
        y: parent.height * 0.1
        width: parent.width * 0.3
        height: parent.height * 0.6
        visible: creature2 !== null
        color: "#AA222222"
        Column {
            anchors.centerIn: parent
            spacing: 10 * mainWindow.height / mainWindow.sourceHeight
            GrowingText {
                standardSize: 20
                text: creature2 ? creature2.species : ""
                color: "white"
            }
            ListView {
                id: grimoireRightImages
                orientation: ListView.Horizontal
                width: childrenRect.width
                height: 30 * mainWindow.height / mainWindow.sourceHeight
                delegate: Image {
                    id: creatureRightImage
                    source: modelData === undefined || modelData === null ? "" : "images/" + modelData
                    height: sourceSize.height * mainWindow.height / mainWindow.sourceHeight // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
                    width: sourceSize.height * mainWindow.height / mainWindow.sourceHeight
                }
            }
            CreatureInformation {
                hoveredCreature: creature2
                player: Player {
                    spellPoints: 10000
                }
                positiveColor: "white"
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
        anchors.bottomMargin: 100 * mainWindow.height / mainWindow.sourceHeight
        SequentialAnimation on opacity {
            id: longMessageAnimation
            running: false
            NumberAnimation {
                from: 1
                to: 0
            }
            NumberAnimation {
                from: 0
                to: 1
            }
        }
        SequentialAnimation on opacity {
            id: shortMessageAnimation
            running: false
            NumberAnimation {
                from: 0
                to: 1
            }
        }
    }

    MorzynButton {
        text: qsTr("Fight!", "Fight in Grimoire")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20 * mainWindow.height / mainWindow.sourceHeight
        visible: creature1 && creature2
        onClicked: {
            console.log("Fight " + creature1.species + " against " + creature2.species);
            if (gameService.message !== "")
            {
                longMessageAnimation.start();
            }
            else
            {
                shortMessageAnimation.start();
            }
            gameService.simulateFight(creature1, creature2);
        }
    }
    MorzynButton {
        text: qsTr("Back", "Back in Grimoire")
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20 * mainWindow.height / mainWindow.sourceHeight
        onClicked: {
            gameService.resetGame();
            game.state = "mainMenuState";
        }
    }
}
