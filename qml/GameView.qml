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
    Component {
        id: manaTransferImageComponent
        ManaTransferImage {

        }
    }

    function playMeleeHit()
    {
        if (!mainWindow.soundActivated || !mainWindow.playSounds || os === "sailfish_sick") return;
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
        if (!mainWindow.soundActivated || !mainWindow.playSounds || os === "sailfish_sick") return;
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
        onManaTransfered: {
            var newManaItem = manaTransferImageComponent.createObject(gameCreatureParentItem);
            if (newManaItem)
            {
                newManaItem.attackingCreature = attackingCreature;
                newManaItem.attackedCreature = attackedCreature;
                newManaItem.start();
            }
        }
    }

    Connections {
        target: game
        onStateChanged: {
            console.log("B: " + game.state);
            if (game.state === "spellSelectState" && !allreadyShown)
            {
                console.log("placing players");
                gameService.placePlayers();
                allreadyShown = true;
                /*if (os !== "sailfish_sick")
                    titleSound.volume = 0.0;
                else
                    titleSound.pause();*/
                gameService.playTitleSong(false);
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
            width: mainWindow1.myWidth * 5/7
            height: mainWindow1.myHeight

            MenuButton {
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 0.02 * mainWindow1.myWidth
                visible: game.state !== "spellSelectState"
            }

            Rectangle {
                anchors.centerIn: parent
                width: hCount * (((416.0 / hCount) * mainWindow1.myHeight) / mainWindow.sourceHeight)
                height: vCount * (((416.0 / vCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) // Absicht, damit height immer = width
                color: "transparent"
                border.width: 2
                border.color: "black"
                Grid {
                    id: gameViewBackRowGrid
                    anchors.fill: parent
                    rows: vCount
                    columns: hCount
                    Repeater {
                        model: (hCount * vCount)
                        delegate: Rectangle {
                            width: height // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
                            height: ((416.0 / hCount) * mainWindow1.myHeight) / mainWindow.sourceHeight
                            color: gameService.getColorOfEmptyField(index
                                     , game.isLocked
                                     , game.selectedCreature ? game.selectedCreature.x : -1
                                     , game.selectedCreature ? game.selectedCreature.y : -1
                                     , game.selectedCreature
                                     , game.currentPlayer
                                     , game.state
                                     , game.counter)//"transparent"
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
            width: mainWindow1.myWidth * 2/7
            height: mainWindow1.myHeight
            /*Text {
                anchors.topMargin: (80 * mainWindow1.myHeight) / mainWindow.sourceHeight
                anchors.bottomMargin: (80 * mainWindow1.myHeight) / mainWindow.sourceHeight
                anchors.leftMargin: (30 * mainWindow.myWidth) / mainWindow.sourceWidth
                anchors.rightMargin: (30 * mainWindow.myWidth) / mainWindow.sourceWidth
                anchors.fill: parent
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                font.pixelSize: (20 * mainWindow1.myHeight) / mainWindow.sourceHeight
                text: game.state === "moveState" ? game.currentPlayer.name + ", make your move!" : (game.state === "castSpellState" ? game.currentPlayer.name + ", cast your spell!" : "")
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                color: "#00FF00"
                visible: hoveredCreature === undefined
            }*/
            BackButton {
                //x: gameViewBackRowStatusBack.x - width - (20 * mainWindow.myWidth / mainWindow.sourceWidth)
                //y: mainWindow1.myHeight - height - (50 * mainWindow1.myHeight / mainWindow.sourceHeight)
                //anchors.bottomMargin: 10 * mainWindow1.myHeight / mainWindow.sourceHeight
                anchors.bottom: parent.bottom
                anchors.bottomMargin: (80 * mainWindow1.myHeight) / mainWindow.sourceHeight
                anchors.horizontalCenter: parent.horizontalCenter
                visible: game.currentPlayer && !game.currentPlayer.isNPC
                onBackClicked: {
                    if (checkInput()) return;
                    gameService.abort(spellSelectView.selectedCreature);
                }
            }
            Column {
                anchors.topMargin: (80 * mainWindow1.myHeight) / mainWindow.sourceHeight
                anchors.bottomMargin: (80 * mainWindow1.myHeight) / mainWindow.sourceHeight
                anchors.leftMargin: (30 * mainWindow1.myWidth) / mainWindow.sourceWidth
                anchors.rightMargin: (30 * mainWindow1.myWidth) / mainWindow.sourceWidth
                anchors.fill: parent
                visible: hoveredCreature !== null
                enabled: hoveredCreature !== null
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width
                    font.pixelSize: (20 * mainWindow1.myHeight) / mainWindow.sourceHeight
                    //text: hoveredCreature && hoveredCreature.species === "wizard" ? hoveredCreature.name : hoveredCreature ? hoveredCreature.player.name + "'s " + hoveredCreature.species : ""
                    text: hoveredCreature ? hoveredCreature.name : ""
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                }
                Item {
                    height: (20 * mainWindow1.myHeight) / mainWindow.sourceHeight
                    width: 100
                }
                Grid {
                    flow: Grid.LeftToRight
                    columns: 2
                    visible: hoveredCreature !== null && hoveredCreature !== undefined
                    spacing: (10 * mainWindow1.myWidth) / mainWindow.sourceWidth
                    GrowingText {
                        text: hoveredCreature && hoveredCreature.species === "wizard" ? qsTr("Spell Points", "Spell Points in game status view") : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: hoveredCreature && hoveredCreature.species === "wizard" ? hoveredCreature.spellPoints : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: qsTr("Health", "Health in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: hoveredCreature ? hoveredCreature.hp : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: qsTr("Strength", "Strength in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: hoveredCreature ? hoveredCreature.strength : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: qsTr("Defense", "Defense in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: hoveredCreature ? hoveredCreature.defense : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: qsTr("Speed", "Speed in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: hoveredCreature ? hoveredCreature.speed : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: qsTr("Remaining", "Remaining move points in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: hoveredCreature ? hoveredCreature.remainingMovePoints : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: !hoveredCreature || hoveredCreature.species === "wizard" || !hoveredCreature.hasDistanceAttack ? "" : qsTr("Dist. Range", "Distance Range in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: !hoveredCreature || hoveredCreature.species === "wizard" ? "" : hoveredCreature.hasDistanceAttack ? hoveredCreature.distanceRange : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: !hoveredCreature || hoveredCreature.species === "wizard" || !hoveredCreature.hasDistanceAttack ? "" : qsTr("Dist. Strength", "Distance Strength in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: !hoveredCreature || hoveredCreature.species === "wizard" ? "" : hoveredCreature.hasDistanceAttack ? hoveredCreature.distanceStrength : ""
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: (!hoveredCreature || hoveredCreature.immune === "") ? "" : qsTr("Immune", "Immune in game status view")
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                    GrowingText {
                        text: (!hoveredCreature || hoveredCreature.immune === "") ? "" : hoveredCreature.immune
                        color: (hoveredCreature && hoveredCreature.player === game.currentPlayer) ? "#FFFFFF" : "#AAAAAA"
                    }
                }
            }
        }
    }
    SpellSelectView {
        id: spellSelectView
        visible: game.state === "spellSelectState" && game.currentPlayer !== null && !game.currentPlayer.isNPC
        onVisibleChanged: {
            if (visible)
            {
                gameView.forceActiveFocus();
            }
        }
    }
    GrowingText {
        text: gameService.message
        standardSize: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        horizontalAlignment: Text.AlignHCenter
        anchors.margins: 10 * mainWindow1.myHeight / mainWindow.sourceHeight
        font.family: "VivaldiD"
        color: "white"
        visible: game && game.currentPlayer && !game.currentPlayer.isNPC
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        propagateComposedEvents: true
        onClicked: {
            if (checkInput()) return;
            console.log("gameViewRightClick");
            gameService.abort(spellSelectView.selectedCreature);
            mouse.accepted = false;
        }
    }
    InGameMenuView {
        id: inGameMenuView
        visible: false
    }
    focus: true
    onActiveFocusChanged: {
        console.debug("active focus gameView = " + activeFocus);
    }

    Rectangle {
        id: cheatBox
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#AA000000"
        visible: false
        height: (30 * mainWindow1.myHeight) / mainWindow.sourceHeight
        width: parent.width / 2
        TextInput {
            id: cheatInput
            anchors.fill: parent
            color: "white"
            font.pixelSize: (30 * mainWindow1.myHeight) / mainWindow.sourceHeight
            onAccepted: {
                gameService.checkCheat(cheatInput.text);
                cheatBox.visible = false;
                cheatInput.text = "";
            }
        }
    }
    MultiPointTouchArea {
        id: cheatTouchArea
        anchors.top: parent.top
        anchors.right: parent.right
        width: parent.width / 5
        height: parent.height / 2
        maximumTouchPoints: 3
        minimumTouchPoints: 3
        onPressed: {
            cheatBox.visible = true;
            accept = false;
        }
    }

    property string cheat
    Keys.enabled: true
    Keys.onPressed: {
        console.log("gedrueckt: " + event.text);
        if (event.key === Qt.Key_Backspace)
        {
            if (checkInput()) return;
            gameService.abort(spellSelectView.selectedCreature);
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
