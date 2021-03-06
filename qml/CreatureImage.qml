// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
//import QtQuick.Particles 2.0
//import Qt.labs.particles 1.0

Rectangle {
    id: creatureImageRect
    color: currentColor
    width: creatureImage.width
    height: creatureImage.height
    x: modelData === undefined || modelData === null ? 0 : modelData.xField * (((416.0 / hCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
    y: modelData === undefined || modelData === null ? 0 : modelData.yField * (((416.0 / vCount) * mainWindow1.myHeight) / mainWindow.sourceHeight)
    property bool isDying: false

    property int movements: 0

    property string startype: "staryellow.png"

    property color markColor: "#66000000"
    property color noColor: "transparent"
    property color selectedColor: "#88CCCCCC"
    property color doneColor: "#AACCCC00"
    property color currentColor: game.currentPlayer && modelData.player && game.currentPlayer === modelData.player ? markColor : noColor

    border.width: 3
    border.color: "transparent"

    property var modelData

    Binding {

    }

    property bool directionLeft: false

    Component.onDestruction: {
    }

    Behavior on x {
        enabled: game.hasBegun && movements > 0
        SequentialAnimation {
            alwaysRunToEnd: true
            PauseAnimation { duration: game.isCreatureDying ? 2000 : 1 }
            PropertyAnimation {
                duration: 300
            }
            onRunningChanged: {
                console.log("x onRunningChanged");
                if (!running)
                {
                    console.log("aa game.isCreatureDying: " + game.isCreatureDying);
                    game.isCreatureDying = false;
                    console.log("bb game.isCreatureDying: " + game.isCreatureDying);
                    console.log("endeee");
                    console.log("x animation release");
                    game.release();
                }
                else
                {
                    console.log("x animation aquire");
                    console.log("game.isCreatureDying: " + game.isCreatureDying);
                    game.aquire();
                    console.log("x animation aquire complete");
                }
            }
        }
    }
    Behavior on y {
        enabled: game.hasBegun && movements > 0
        SequentialAnimation {
            alwaysRunToEnd: true
            PauseAnimation { duration: game.isCreatureDying ? 2000 : 1 }
            PropertyAnimation {
                duration: 300
            }
            onRunningChanged: {
                console.log("y onRunningChanged");
                if (!running)
                {
                    game.isCreatureDying = false;
                    console.log("endeee");
                    console.log("y animation release");
                    game.release();
                }
                else
                {
                    console.log("y animation aquire");
                    console.log("y game.isCreatureDying: " + game.isCreatureDying);
                    game.aquire();
                }
            }
        }
    }
    signal creatureClicked

    //color: (game.selectedCreature === modelData) ? "#AA000000" : "transparent";
    //property bool selected: false

    Connections {
        target: game
        onSelectedCreatureChanged: {
            color = (game.selectedCreature === modelData) ? selectedColor : currentColor;
        }
        onCurrentPlayerChanged: {
            color = (game.selectedCreature === modelData) ? selectedColor : currentColor;
        }
    }

    Image {
        id: creatureImage
        source: modelData === undefined || modelData === null ? "" : "images/" + modelData.imageFilename
        height: (416.0 / vCount) * mainWindow1.myHeight / mainWindow.sourceHeight // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
        width: (416.0 / vCount) * mainWindow1.myHeight / mainWindow.sourceHeight
        SequentialAnimation on rotation {
            id: attackAnimation
            running: false
            RotationAnimation {
                from: 0
                to: directionLeft ? -15 : 15
                duration: 250
            }
            RotationAnimation {
                from: directionLeft ? -15 : 15
                to: 0
                duration: 250
            }
            onRunningChanged: {
                if (!running)
                {
                    console.log("rotation animation release");
                    game.release();
                }
                else
                {
                    console.log("rotation animation aquire");
                    game.aquire();
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            propagateComposedEvents: true
            onClicked: {
                if (checkInput()) return;
                if (game.state === "castSpellState")
                {
                    gameService.tryCast(modelData.x, modelData.y, spellSelectView.selectedCreature);
                }
                else {
                    gameService.trySelectCreature(modelData);
                    gameService.tryAttacks(modelData);
                }
            }
        }
    }
    Image {
        id: creatureDamageStar
        source: "images/" + startype
        width: creatureImage.width
        height: creatureImage.height
        scale: 0
        Text {
            id: creatureDamageStarText
            anchors.centerIn: parent
            font.pixelSize: (12 * mainWindow1.myHeight) / mainWindow.sourceHeight
        }
        SequentialAnimation on scale {
            id: creatureDamageStarAnimation
            alwaysRunToEnd: true
            running: false
            NumberAnimation {
                from: 0
                to: 1
                duration: 300
            }
            PauseAnimation {
                duration: 200
            }
            NumberAnimation {
                from: 1
                to: 0
                duration: 300
            }
            onRunningChanged: {
                if (!running)
                {
                    console.log("State " + creatureImageRect.state);
                    if (isDying)
                    {
                        console.log("Oha, der ist tot...starte Sterbeanimation");
                        creatureDeadAnimation.running = true;
                        if (modelData === modelData.player)
                        {
                            gameService.emitPlayersCreaturesDie(modelData.player);
                        }
                    }
                    console.log("scale animation release");
                    game.release();
                }
                else
                {
                    //game.aquire();
                }
            }
        }
    }

    SequentialAnimation on scale {
        id: creatureDeadAnimation
        alwaysRunToEnd: true
        running: false
        NumberAnimation {
            from: 1
            to: 0
            duration: 800
        }
        onRunningChanged: {
            if (!running)
            {
                scale = 0;
                visible = false;
                console.log("die animation release");
                game.release();
            }
            else
            {
                console.log("die animation aquire");
                game.aquire();
            }
        }
    }

    Connections {
        target: game
        onStateChanged: {
            // Abfrage auf NPC, um die ANimation nicht doppelt zu starten
            // die würde sonst gleichzeitig laufen mit der Bewegungsblinkerei
            if ((game.state === "castSpellState" && !game.currentPlayer.isNPC) || game.state === "moveState")
            {
                if (modelData.player === game.currentPlayer)
                {
                    //particleEmitter.burst(200, -1);
                    //particleEmitter.pulse(1000);
                    startColorAnimation();
                }
            }
        }
    }

    property bool manaTransfer: false

    Connections {
        target: gameService

        onCreatureHealed: {
            if (creature === modelData)
            {
                healingHeart.healingAmount = hp;
                healingHeart.start();
            }
        }

        onManaTransfered: {
            if (attackedCreature === modelData)
            {
                manaTransfer = true;
            }
        }

        onCreatureAttacked: {
            if (attackedCreature === modelData)
            {
                console.log("Schaden: " + nDamage);
                creatureDamageStarText.text = nDamage;
                startype = "staryellow.png";
                if (attackingCreature.strength <= nDamage)
                {
                    startype = "starred.png";
                }
                else if (nDamage === 0)
                {
                    startype = "starwhite.png";
                }
                creatureDamageStarAnimation.start();
                console.log("scale animation aquire");
                game.aquire();
                if (nDamage > 0)
                {
                    playMeleeHit();
                }
                else
                {
                    playMeleeMiss();
                }
            }
            else if (attackingCreature === modelData)
            {
                if (attackingCreature.x < attackedCreature.x)
                {
                    directionLeft = false;
                }
                else
                {
                    directionLeft = true;
                }

                console.log("Ich bin der Angreifer und verursache Schaden: " + nDamage);
                attackAnimation.start();
            }
        }
        onGameReset: {
            console.log("Game Resetttettet");
            creatureImageRect.destroy();
        }
        onCreatureMoved: {
            if (creature === modelData)
            {
                console.log("received Creature moved");
                movements++;
            }
        }
        onPlayersCreaturesDie: {
            if (player === modelData.player)
            {
                creatureDeadAnimation.start();
            }
        }
    }

    Connections {
        target: modelData
        ignoreUnknownSignals: true
        onAliveChanged: {
            if (!modelData.alive)
            {
                isDying = true;
                console.log("Alive: " + modelData.alive);
            }
        }
    }

    Connections {
        target: gameView
        onDistanceAnimationFinished: {
            if (attackedCreature === modelData)
            {
                console.log("Distanz-Schaden: " + nDamage);
                creatureDamageStarText.text = nDamage;
                startype = "staryellow.png";
                var immune = false;
                if (attackingCreature.immune && attackingCreature.immune !== "")
                {
                    if (attackedCreature.immune === "ALL")
                    {
                        immune = true;
                    }
                    else if (attackingCreature.immune && attackedCreature.immune && attackingCreature.immune === attackedCreature.immune)
                    {
                        immune = true;
                    }
                }
                if (!manaTransfer && immune)
                {
                    startype = "stargreen.png";
                }
                else if (!manaTransfer && attackingCreature.distanceStrength <= nDamage)
                {
                    startype = "starred.png";
                }
                else if (!manaTransfer && nDamage === 0)
                {
                    startype = "starwhite.png";
                }
                else if (manaTransfer)
                {
                    startype = "starblue.png";
                    manaTransfer = false;
                }

                creatureDamageStarAnimation.start();
            }
        }
    }

    function startColorAnimation()
    {
        creatureColorAnimation.start();
    }

    SequentialAnimation {
        id: creatureColorAnimation
        alwaysRunToEnd: true
        loops: 3
        ColorAnimation {
            from: currentColor
            to: selectedColor
            duration: 100
            target: creatureImageRect
            property: "color"
        }
        ColorAnimation {
            from: selectedColor
            to: currentColor
            duration: 100
            target: creatureImageRect
            property: "color"
        }
        onRunningChanged: {
            if (!running)
            {
                console.log("Stopped Blinking");
                creatureImageRect.color = currentColor;
                console.log("blinking animation release");
                game.release();
            }
            else
            {
                console.log("blinking animation aquire");
                game.aquire();
                console.log("animation started");
            }
        }
    }

    HealingHeart {
        id: healingHeart
    }

    ManaStar {
        id: manaStar
    }

    Component.onCompleted: {
        visible = modelData.alive;
    }
}
