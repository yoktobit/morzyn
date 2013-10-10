// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    id: gameOverView
    property string winner
    anchors.fill: parent
    Image {
        id: mainMenuViewBack
        source: "images/titleback.png"
        anchors.fill: parent
        Text {
            font.family: "VivaldiD"
            font.pixelSize: (50 * mainWindow.height) / mainWindow.sourceHeight
            text: qsTr("%1 wins!", "... wins!").arg(winner)
            anchors.centerIn: parent
        }
    }
    Connections {
        target: game
        onStateChanged: {
            if (game.state === "gameOverState")
            {
                for (var ii = 0; ii < game.players.length; ii++)
                {
                    console.log(game.players[ii].name + " alive? " + game.players[ii].alive);
                    if (game.players[ii].alive)
                        winner = game.players[ii].name;
                }
                gameOverView.opacity = 0;
                gameOverView.visible = true;
                tmrWait.interval = 2000;
                tmrWait.start();
            }
        }
    }
    visible: false
    opacity: 0

    Timer {
        id: tmrWait
        interval: 2000
        repeat: true
        onTriggered: {
            tmrWait.interval = 200;
            if (!game.isLocked)
            {
                tmrWait.interval = 2000;
                tmrWait.stop();
                overlayAnimation.start();
            }
        }
    }

    SequentialAnimation {
        id: overlayAnimation
        PropertyAnimation {
            target: gameOverView
            property: "opacity"
            from: 0
            to: 1
            duration: 200
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            gameOverView.visible = false;
            gameOverView.opacity = 0;
            gameService.resetGame();
            game.state = "mainMenuState";
        }
    }
}
