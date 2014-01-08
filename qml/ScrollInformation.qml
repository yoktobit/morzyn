// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Grid {
    property var hoveredCreature
    property var player
    flow: Grid.LeftToRight
    columns: 2
    visible: hoveredCreature && hoveredCreature !== null && hoveredCreature.unitClass !== "Creature"
    enabled: hoveredCreature !== null
    spacing: (10 * mainWindow.myWidth) / mainWindow.sourceWidth
    GrowingText {
        text: qsTr("Cost", "Cost in scroll information")
        color: (hoveredCreature && game.currentPlayer.spellPoints >= hoveredCreature.manaCost ? "#00FF00" : "red")
    }
    GrowingText {
        text: hoveredCreature ? hoveredCreature.manaCost : ""
        color: (hoveredCreature && game.currentPlayer.spellPoints >= hoveredCreature.manaCost ? "#00FF00" : "red")
    }
    GrowingText {
        text: qsTr("Damage", "Damage in scroll information")
        color: (hoveredCreature && game.currentPlayer.spellPoints >= hoveredCreature.manaCost ? "#00FF00" : "red")
    }
    GrowingText {
        text: hoveredCreature ? hoveredCreature.minDamage + " - " + hoveredCreature.maxDamage : ""
        color: (hoveredCreature && game.currentPlayer.spellPoints >= hoveredCreature.manaCost ? "#00FF00" : "red")
    }
    GrowingText {
        text: hoveredCreature && hoveredCreature.hasDistanceAttack ? qsTr("Range", "Range in scroll information") : ""
        color: (hoveredCreature && game.currentPlayer.spellPoints >= hoveredCreature.manaCost ? "#00FF00" : "red")
    }
    GrowingText {
        text: hoveredCreature ? (hoveredCreature.hasDistanceAttack ? hoveredCreature.range : "") : ""
        color: (hoveredCreature && game.currentPlayer.spellPoints >= hoveredCreature.manaCost ? "#00FF00" : "red")
    }
    GrowingText {
        text: (hoveredCreature && hoveredCreature.immune !== "") ? qsTr("Type", "Type in scroll information") : ""
        color: (game.currentPlayer.spellPoints >= hoveredCreature.manaCost ? "#00FF00" : "red")
    }
    GrowingText {
        text: (hoveredCreature && hoveredCreature.immune !== "") ? hoveredCreature.immune : ""
        color: (game.currentPlayer.spellPoints >= hoveredCreature.manaCost ? "#00FF00" : "red")
    }
}
