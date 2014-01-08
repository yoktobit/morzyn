// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Grid {
    property var hoveredCreature
    property var player
    property string positiveColor: "#00FF00"
    flow: Grid.LeftToRight
    columns: 2
    visible: hoveredCreature !== null && hoveredCreature.unitClass === "Creature"
    enabled: hoveredCreature !== null
    spacing: (10 * mainWindow.myWidth) / mainWindow.sourceWidth
    GrowingText {
        text: qsTr("Cost", "Cost in creature information")
        color: (hoveredCreature && player && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: hoveredCreature !== null ? hoveredCreature.manaCost : ""
        color: (hoveredCreature && player && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: qsTr("Health", "Health in creature information")
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: hoveredCreature ? hoveredCreature.hp : ""
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: qsTr("Strength", "Strength in creature information")
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: hoveredCreature ? hoveredCreature.strength : ""
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: qsTr("Defense", "Defense in creature information")
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: hoveredCreature ? hoveredCreature.defense : ""
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: qsTr("Speed", "Speed in creature information")
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: hoveredCreature ? hoveredCreature.speed : ""
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: hoveredCreature && hoveredCreature.hasDistanceAttack ? qsTr("Dist. Range", "Dist Range in creature information") : ""
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: hoveredCreature ? (hoveredCreature.hasDistanceAttack ? hoveredCreature.distanceRange : "") : ""
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: hoveredCreature && hoveredCreature.hasDistanceAttack ? qsTr("Dist. Strength", "Distance Strength in creature information") : ""
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: hoveredCreature ? (hoveredCreature.hasDistanceAttack ? hoveredCreature.distanceStrength : "") : ""
        color: (hoveredCreature && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: (hoveredCreature && hoveredCreature.immune !== "") ? qsTr("Immune", "Immune in creature information") : ""
        color: (player && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
    GrowingText {
        text: (hoveredCreature && hoveredCreature.immune !== "") ? hoveredCreature.immune : ""
        color: (player && player.spellPoints >= hoveredCreature.manaCost ? positiveColor : "red")
    }
}
