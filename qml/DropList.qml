// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    id: dropList
    property string selectedItem

    property alias model: dropListListView.model

    width: (100 * mainWindow.myWidth) / mainWindow.sourceWidth
    height: (25 * mainWindow.myHeight) / mainWindow.sourceHeight
    border.width: 1
    border.color: "red"
    color: "black"
    Text {
        id: dropListText
        //width: (100 * mainWindow.myWidth) / mainWindow.sourceWidth
        font.pixelSize: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
        anchors.centerIn: parent
        color: "red"
        text: selectedItem
        MouseArea {
            anchors.fill: parent
            onClicked: {
                dropList.state = "open";
                //dropList.focus = true;
            }
        }
    }

    states: [
        State {
            name: "open"
            PropertyChanges {
                target: dropList
                height: dropListColumn.height
            }
            PropertyChanges {
                target: dropListColumn
                visible: true
            }
            PropertyChanges {
                target: dropListText
                visible: false
            }
        }
    ]

    onStateChanged: {
        console.log(dropListListView.height);
    }
    onFocusChanged: {
        if (!focus) {
            dropList.state = "";
        }
    }

    Column {
        id: dropListColumn
        anchors.centerIn: parent
        visible: false
        spacing: 10
        Repeater {
            id: dropListListView
            delegate: Text {
                color: "red"
                anchors.horizontalCenter: parent.horizontalCenter
                text: name
                font.pixelSize: (20 * mainWindow.myHeight) / mainWindow.sourceHeight
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Selected " + name);
                        selectedItem = name;
                        playerColorSelector.current = (selectedItem != "dark priest" ? Math.min(playerColorSelector.current, 9) : playerColorSelector.current);
                        playerColorSelector.set();
                        dropList.state = "";
                    }
                }
            }
        }
    }
}
