import QtQuick 2.0

Rectangle {
    width: mainWindow.width * 0.05;
    height: width
    color: "#DDDDDD"
    border.width: 2
    border.color: "#888888"
    property bool checked
    Image {
        source: "images/kreuz.png"
        anchors.fill: parent
        anchors.margins: 1
        visible: checked
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            checked = !checked;
        }
    }
}
