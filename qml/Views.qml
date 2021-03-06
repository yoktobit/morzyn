import QtQuick 2.1
import QtMultimedia 5.0
import harbour.morzyn 1.0

Item {
    id: allViews
    anchors.fill: parent
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
        onVisibleChanged: {
            if (visible) totalPlayerCountView.forceActiveFocus();
        }
    }
    PlayerCountView {
        id: humanPlayerCountView
        type: "human"
        visible: game.state === "humanPlayerCountState"
        min: 0
        max: totalPlayerCountView.selected
        onVisibleChanged: {
            if (visible) humanPlayerCountView.forceActiveFocus();
        }
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
                console.debug("gameView spellSelectState");
                gameView.visible = true;
                gameView.focus = true;
                console.debug("gameView activeFocus " + gameView.activeFocus);
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
            if (visible) gameView.focus = true;
        }
    }
    GameOverView {
        id: gameOverView
    }

    Component.onCompleted: {
        //console.log(morzyn.Spells.Creatures.Creature[0].name);
        console.log("Library count: " + library.creatures.length);
    }

    property bool fullscreen: gameService.getBoolSetting("fullscreen");

    focus: true
    Keys.enabled: true
    Keys.onPressed: {
        console.log("Views KEY_PRESSED: " + event.key)
        if ((event.key === Qt.Key_PageUp))
        {
            fullscreen = !fullscreen;
            gameService.setFullScreen(fullscreen);
        }
        else if (event.key === Qt.Key_Escape)
        {
            console.log("Escape pressed");
            if (!game.hasBegun && game.state !== "mainMenuState")
            {
                gameService.resetGame();
                game.state = "mainMenuState";
            }
        }
    }

    Keys.onReleased: {
        console.log("Views KEY_RELEASED: " + event.key)
        if ((event.key === Qt.BackButton) || event.key == Qt.Key_Back || event.key == Qt.Key_MediaPrevious || event.key == 16777313)
        {
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
