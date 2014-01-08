// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    property int current: 0
    //property alias playerColors: playerColors
    width: playerColorSelectorImage.width
    height: playerColorSelectorImage.height
    color: "transparent"
    function set()
    {
        console.log("current: " + current + "; category: " + playerColorCategory.selectedItem);
        gameService.setCreatureImage(index, current, playerColorCategory.selectedItem);
    }

    Image {
        id: playerColorSelectorImage
        source: "images/" + imageFilename // playerColors.get(current).file
        width: height //* sourceSize.width / sourceSize.height
        height: (sourceSize.height * mainWindow.myHeight) / mainWindow.sourceHeight
        anchors.centerIn: parent
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            onClicked: {
                current = (current + 1) % (playerColorCategory.selectedItem === "dark priest" ? 14 : 10);
                set();
            }
        }
    }
    Component.onCompleted: {
        current = gameService.getColorIndex(imageFilename);
    }
}
