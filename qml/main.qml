// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
import harbour.de.yoktobit.morzyn 1.0
import QtMultimedia 5.0

Rectangle {

    width: 693
    height: 499
    id: mainWindow

    property int sourceWidth: 693
    property int sourceHeight: 499

    property bool playSounds: true

    Loader {
        anchors.fill: parent
        source: "Views.qml"
        focus: true
        asynchronous: true
        visible: status == Loader.Ready
    }

    TitleView {
        visible: game.state === "titleScreenState"
    }

    Audio {
        id: titleSound
        source: "sounds/morzyn intro.mp3"
        autoPlay: true
        loops: Audio.Infinite
        Behavior on volume {
            PropertyAnimation {
                duration: 2000
                onRunningChanged: {
                    if (!running)
                    {
                        if (titleSound.volume === 0)
                        {
                            titleSound.pause();
                        }
                    }
                }
            }
        }
    }
    /*
    MainMenuView {
        visible: game.state === "mainMenuState"
    }
    PlayerCountView {
        id: totalPlayerCountView
        type: "total"
        visible: game.state === "totalPlayerCountState"
        min: 2
        max: 7
    }
    PlayerCountView {
        id: humanPlayerCountView
        type: "human"
        visible: game.state === "humanPlayerCountState"
        min: 0
        max: totalPlayerCountView.selected
    }
    PlayerNameView {
        id: playerNameView
        visible: game.state === "playerNameState"
    }
    Connections {
        target: game
        onStateChanged: {
            if (game.state === "spellSelectState")
            {
                gameView.visible = true;
                gameView.focus = true;
            }
            else if (game.state === "mainMenuState")
            {
                gameView.visible = false;
                gameView.allreadyShown = false;
            }
        }
    }
    GrimoireView {
        id: grimoireView
        visible: game.state === "grimoireState"
    }

    GameView {
        id: gameView
        visible: false
        onVisibleChanged: {
            console.log(visible);
        }
    }
    GameOverView {
        id: gameOverView
    }

    Audio {
        id: titleSound
        source: "sounds/morzyn intro.mp3"
        autoPlay: true
        loops: Audio.Infinite
        Behavior on volume {
            PropertyAnimation {
                duration: 2000
                onRunningChanged: {
                    if (!running)
                    {
                        if (titleSound.volume === 0)
                        {
                            titleSound.pause();
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: game
        onStateChanged: {
            // Neustart beim Zurückkehren
            if (game.state === "mainMenuState")
            {
                if (!titleSound.playing())
                {
                    titleSound.volume = 1.0;
                    titleSound.play();
                }
            }
        }
    }*/

    Component.onCompleted: {
        library.loadAll();
    }

    /*property bool fullscreen: false
    Keys.onPressed: {
        if ((event.key === Qt.Key_PageUp))
        {
            fullscreen = !fullscreen;
            gameService.setFullScreen(fullscreen);
        }
    }*/
}
