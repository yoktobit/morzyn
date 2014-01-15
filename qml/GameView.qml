// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
import QtMultimedia 5.0
//import "service.js" as Service

Rectangle {
    id: gameView
    anchors.centerIn: parent
    width: gameViewBackRowStatusBack.width + gameViewBackRowWiese.width
    height: gameViewBackRowWiese.height

    property bool allreadyShown: false

    property var hoveredCreature: null

    signal distanceAnimationFinished(var attackingCreature, var attackedCreature, int nDamage)

    property InGameMenuView inGameMenuViewExternal: inGameMenuView

    property var creatures: [];

    Component {
        id: creatureComponent
        CreatureImage {
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                hoverEnabled: true
                onEntered: {
                    hoveredCreature = modelData;
                }
                onExited: {
                    hoveredCreature = null;
                }
            }
        }
    }
    Component {
        id: scrollComponent
        Scroll {

        }
    }

    function playMeleeHit()
    {
        if (!mainWindow.playSounds || os === "sailfish") return;
        var randomnumber = Math.floor(Math.random() * 5);
        console.log("MeleeHit " + randomnumber);
        switch (randomnumber)
        {
        case 0:
            meleeHit1.play();
            console.log("melee1");
            break;
        case 1:
            meleeHit2.play();
            console.log("melee2");
            break;
        case 2:
            meleeHit3.play();
            console.log("melee3");
            break;
        case 3:
            meleeHit4.play();
            console.log("melee4");
            break;
        case 4:
            meleeHit5.play();
            console.log("melee5");
            break;
        }
    }

    function playMeleeMiss()
    {
        if (!mainWindow.playSounds || os === "sailfish") return;
        var randomnumber = Math.floor(Math.random() * 3);
        console.log("MeleeMiss " + randomnumber);
        switch (randomnumber)
        {
        case 0:
            meleeMiss1.play();
            break;
        case 1:
            meleeMiss2.play();
            break;
        case 2:
            meleeMiss3.play();
            break;
        }
    }

    function checkInput()
    {
        if (game.currentPlayer && game.currentPlayer.isNPC)
        {
            console.log("Player " + game.currentPlayer.name + " is NPC, cannot abort");
            return true;
        }
        if (game.isLocked)
        {
            console.log("Game is locked");
            return true;
        }
        return false;
    }

    Connections {
        target: gameService
        onCreatureDistanceAttacked: {
            console.log("gameView: received onCreatureDistanceAttacked");
            gameDistanceAttackImage.nDamage = nDamage;
            gameDistanceAttackImage.attackingCreature = attackingCreature;
            gameDistanceAttackImage.attackedCreature = attackedCreature;
            gameDistanceAttackImage.start();
        }
        onCreatureCasted: {
            console.log("gameView: received onCreatureCasted");
            var newObject = creatureComponent.createObject(gameCreatureParentItem);
            if (newObject)
            {
                creatures.push(newObject);
                newObject.modelData = creature;
            }
        }
    }

    Connections {
        target: game
        onStateChanged: {
            console.log("B: " + game.state);
            if (game.state === "spellSelectState" && !allreadyShown)
            {
                console.log("vorm placen ImageFilename Player 0" + game.creatures[0].imageFilename);
                console.log("placing players");
                gameService.placePlayers();
                console.log("ImageFilename Player 0" + game.creatures[0].imageFilename);
                allreadyShown = true;
                if (os !== "sailfish")
                    titleSound.volume = 0.0;
                else
                    titleSound.pause();
            }
        }
    }

    SoundEffect {
        id: meleeHit1
        source: "sounds/meleehit01.wav"
    }
    SoundEffect {
        id: meleeHit2
        source: "sounds/meleehit02.wav"
    }
    SoundEffect {
        id: meleeHit3
        source: "sounds/meleehit03.wav"
    }
    SoundEffect {
        id: meleeHit4
        source: "sounds/meleehit04.wav"
    }
    SoundEffect {
        id: meleeHit5
        source: "sounds/meleehit05.wav"
    }
    SoundEffect {
        id: meleeMiss1
        source: "sounds/meleemiss01.wav"
    }
    SoundEffect {
        id: meleeMiss2
        source: "sounds/meleemiss02.wav"
    }
    SoundEffect {
        id: meleeMiss3
        source: "sounds/meleemiss03.wav"
    }

    function emptyFieldClicked(index)
    {
        var y = index / vCount;
        var x = index % hCount;
        if (game.state === "castSpellState")
        {
            gameService.tryCast(x, y, spellSelectView.selectedCreature);
        }
        if (game.state === "moveState")
        {
            gameService.tryMoveCreature(game.selectedCreature, x, y);
        }
    }

    Row {
        id: gameViewBackRow
        anchors.fill: parent
        Image {
            id: gameViewBackRowWiese
            source: "images/wiese.png"
            width: mainWindow.myWidth * 5/7
            height: mainWindow.myHeight

            MenuButton {
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 0.02 * mainWindow.myWidth
                visible: game.state !== "spellSelectState"
            }

            Grid {
                id: gameViewBackRowGrid
                anchors.centerIn: parent
                width: hCount * (((416.0 / hCount) * mainWindow.myHeight) / mainWindow.sourceHeight)
                height: vCount * (((416.0 / vCount) * mainWindow.myHeight) / mainWindow.sourceHeight) // Absicht, damit height immer = width
                rows: vCount
                columns: hCount
                Repeater {
                    model: (hCount * vCount)
                    delegate: Rectangle {
                        width: height // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
                        height: ((416.0 / hCount) * mainWindow.myHeight) / mainWindow.sourceHeight
                        color: "transparent"
                        border.width: 1
                        border.color: "black"
                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton
                            propagateComposedEvents: true
                            onClicked: {
                                if (checkInput()) return;
                                emptyFieldClicked(index);
                            }
                        }
                    }
                }
            }
            Item {
                id: gameCreatureParentItem
                anchors.centerIn: parent
                width: gameViewBackRowGrid.width
                height: gameViewBackRowGrid.height
                DistanceAttackImage {
                    id: gameDistanceAttackImage
                    visible: false
                    onAnimationFinished: {
                        distanceAnimationFinished(attackingCreature, attackedCreature, nDamage);
                    }
                }
            }
            Item {
                anchors.centerIn: parent
                width: gameViewBackRowGrid.width
                height: gameViewBackRowGrid.height
                Repeater {
                    id: gameViewScrollRepeater
                    model: game.scrolls
                    delegate: scrollComponent
                }
            }
        }
        Image {
            id: gameViewBackRowStatusBack
            source: "images/statusback.png"
            width: mainWindow.myWidth * 2/7
            height: mainWindow.myHeight
            /*Text {
                anchors.topMargin: (80 * mainWindow.myHeight) / mainWindow.sourceHeight
                anchors.bottomMargin: (80 * mainWindow.myHeight) / mainWindow.sourceHeight
                anchors.leftMargin: (30 * mainWindow.myWidth) / mainWindow.sourceWidth
                anchors.rightMargin: (30 * mainWindow.myWidth) / mainWindow.sourceWidth
                anchors.fill: parent
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                font.pixelSize: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
                text: game.state === "moveState" ? game.currentPlayer.name + ", make your move!" : (game.state === "castSpellState" ? game.currentPlayer.name + ", cast your spell!" : "")
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                color: "#00FF00"
                visible: hoveredCreature === undefined
            }*/
            BackButton {
                //x: gameViewBackRowStatusBack.x - width - (20 * mainWindow.myWidth / mainWindow.sourceWidth)
                //y: mainWindow.myHeight - height - (50 * mainWindow.myHeight / mainWindow.sourceHeight)
                //anchors.bottomMargin: 10 * mainWindow.myHeight / mainWindow.sourceHeight
                anchors.bottom: parent.bottom
                anchors.bottomMargin: (80 * mainWindow.myHeight) / mainWindow.sourceHeight
                anchors.horizontalCenter: parent.horizontalCenter
                onBackClicked: {
                    if (checkInput()) return;
                    gameService.abort();
                }
            }
            Column {
                anchors.topMargin: (80 * mainWindow.myHeight) / mainWindow.sourceHeight
                anchors.bottomMargin: (80 * mainWindow.myHeight) / mainWindow.sourceHeight
                anchors.leftMargin: (30 * mainWindow.myWidth) / mainWindow.sourceWidth
                anchors.rightMargin: (30 * mainWindow.myWidth) / mainWindow.sourceWidth
                anchors.fill: parent
                visible: hoveredCreature !== null
                enabled: hoveredCreature !== null
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    font.pixelSize: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
                    text: hoveredCreature && hoveredCreature.species === "wizard" ? hoveredCreature.name : hoveredCreature ? hoveredCreature.player.name + "'s " + hoveredCreature.species : ""
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                }
                Item {
                    height: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
                    width: 100
                }
                Grid {
                    flow: Grid.LeftToRight
                    columns: 2
                    visible: hoveredCreature !== null && hoveredCreature !== undefined
                    spacing: (10 * mainWindow.myWidth) / mainWindow.sourceWidth
                    GrowingText {
                        text: hoveredCreature && hoveredCreature.species === "wizard" ? qsTr("Spell Points", "Spell Points in game status view") : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: hoveredCreature && hoveredCreature.species === "wizard" ? hoveredCreature.spellPoints : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: qsTr("Health", "Health in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: hoveredCreature ? hoveredCreature.hp : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: qsTr("Strength", "Strength in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: hoveredCreature ? hoveredCreature.strength : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: qsTr("Defense", "Defense in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: hoveredCreature ? hoveredCreature.defense : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: qsTr("Speed", "Speed in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: hoveredCreature ? hoveredCreature.speed : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: qsTr("Remaining", "Remaining move points in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: hoveredCreature ? hoveredCreature.remainingMovePoints : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: !hoveredCreature || hoveredCreature.species === "wizard" || !hoveredCreature.hasDistanceAttack ? "" : qsTr("Dist. Range", "Distance Range in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: !hoveredCreature || hoveredCreature.species === "wizard" ? "" : hoveredCreature.hasDistanceAttack ? hoveredCreature.distanceRange : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: !hoveredCreature || hoveredCreature.species === "wizard" || !hoveredCreature.hasDistanceAttack ? "" : qsTr("Dist. Strength", "Distance Strength in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: !hoveredCreature || hoveredCreature.species === "wizard" ? "" : hoveredCreature.hasDistanceAttack ? hoveredCreature.distanceStrength : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: (!hoveredCreature || hoveredCreature.immune === "") ? "" : qsTr("Immune", "Immune in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                    GrowingText {
                        text: (!hoveredCreature || hoveredCreature.immune === "") ? "" : hoveredCreature.immune
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#00FF00" : "red"
                    }
                }
            }
        }
    }
    SpellSelectView {
        id: spellSelectView
        visible: game.state === "spellSelectState" && game.currentPlayer !== null && !game.currentPlayer.isNPC
    }
    GrowingText {
        text: gameService.message
        standardSize: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        horizontalAlignment: Text.AlignHCenter
        anchors.margins: 10 * mainWindow.myHeight / mainWindow.sourceHeight
        font.family: "VivaldiD"
        color: "white"
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        propagateComposedEvents: true
        onClicked: {
            if (checkInput()) return;
            console.log("gameViewRightClick");
            gameService.abort();
            mouse.accepted = false;
        }
    }
    InGameMenuView {
        id: inGameMenuView
        visible: false
    }
    focus: true
    property string cheat
    Keys.onPressed: {
        console.log("gedrueckt: " + event.text);
        if (event.key === Qt.Key_Backspace)
        {
            if (checkInput()) return;
            gameService.abort();
        }
        else if (event.key === Qt.Key_Escape && (game.state === "spellSelectState" || game.state === "moveState" || game.state === "castSpellState"))
        {
            inGameMenuView.visible = true;
        }
        else
        {
            if (checkInput()) return;
            gameService.checkCheat(event.text);
        }
    }
}
