// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    id: theswitch
    property ListModel model
    property int currentIndex: 0
    property string selectedItem: model.get(currentIndex).name
    property string selectedValue: model.get(currentIndex).value

    width: mainWindow.width * 0.25//(100 * mainWindow.myWidth) / mainWindow.sourceWidth
    height: width * 0.2//(25 * mainWindow.myHeight) / mainWindow.sourceHeight
    border.width: 1
    border.color: "red"
    color: "black"

    function sync()
    {
        for (var ii = 0; ii < model.count; ii++)
        {
            if (model.get(ii).value === selectedValue)
            {
                currentIndex = ii;
                console.debug("currentIndex synced " + ii);
                return;
            }
        }
    }

    Text {
        id: dropListText
        font.pixelSize: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
        anchors.centerIn: parent
        color: "red"
        text: selectedItem   
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("Selected " + model.value);
            currentIndex++;
            currentIndex = currentIndex % model.count;
            selectedItem = model.get(currentIndex).name;
            selectedValue = model.get(currentIndex).value;
            if (playerColorSelector)
            {
                playerColorSelector.current = (selectedValue != "dark priest" ? Math.min(playerColorSelector.current, 9) : playerColorSelector.current);
                playerColorSelector.set();
            }
        }
    }
    Component.onCompleted: {
        console.debug("Switch completed with " + selectedValue);
        sync();
    }
}
