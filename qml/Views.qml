import QtQuick 2.1
import QtMultimedia 5.0
import harbour.morzyn 1.0

Item {
    id: allViews
    anchors.fill: parent
    focus: true
    FontLoader {
        source: "fonts/vivaldi.ttf"
    }
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
    OptionsView {
        id: optionsView
        visible: game.state === "optionsState"
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

    Connections {
        target: game
        onStateChanged: {
            // Neustart beim Zurückkehren
            if (game.state === "mainMenuState")
            {
                /*if (!titleSound.playing() && mainWindow.applicationActive)
                {
                    if (os !== "sailfish")
                        titleSound.volume = 1.0;
                    titleSound.play();
                }*/
                if (mainWindow.applicationActive)
                {
                    gameService.playTitleSong(true);
                }
            }
        }
    }

    Component.onCompleted: {
        //console.log(morzyn.Spells.Creatures.Creature[0].name);
        //gameService.playTitleSong(true);
        console.log("Library count: " + library.creatures.count);
    }

    property bool fullscreen: gameService.getBoolSetting("fullscreen");
    Keys.onPressed: {
        console.log("KEY_PRESSED: " + event.key)
        if ((event.key === Qt.Key_PageUp))
        {
            fullscreen = !fullscreen;
            gameService.setFullScreen(fullscreen);
        }
    }

    Keys.onReleased: {
        console.log("KEY_PRESSED: " + event.key)
        if ((event.key === Qt.BackButton) || event.key == Qt.Key_Back || event.key == Qt.Key_MediaPrevious || event.key == 16777313)
        {
            event.accepted = true;
            if (game.hasBegun)
            {
                console.debug("calling in-game-menu");
                gameView.inGameMenuViewExternal.visible = true;
            }
            else
            {
                console.debug("directly back to menu");
                gameService.resetGame();
                game.state = "mainMenuState";
            }
        }
    }
}
