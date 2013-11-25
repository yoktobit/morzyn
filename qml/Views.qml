import QtQuick 2.1
import QtMultimedia 5.0
import harbour.de.yoktobit.morzyn 1.0

Item {
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
                if (!titleSound.playing() && morzynApp.applicationActive)
                {
                    titleSound.volume = 1.0;
                    titleSound.play();
                }
            }
        }
    }

    Component.onCompleted: {
        //console.log(morzyn.Spells.Creatures.Creature[0].name);
        //gameService.playTitleSong(true);
        console.log("Library count: " + library.creatures.count);
    }

    property bool fullscreen: false
    Keys.onPressed: {
        if ((event.key === Qt.Key_PageUp))
        {
            fullscreen = !fullscreen;
            gameService.setFullScreen(fullscreen);
        }
    }
}
