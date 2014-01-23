import QtQuick 2.0

Item {
    id: optionsView
    anchors.fill: parent
    Image {
        source: "images/titleback.png"
        anchors.fill: parent
    }
    Rectangle {
        anchors.centerIn: parent
        width: parent.width / 2
        height: parent.height / 2
        color: "#AA222222"
        Column {
            id: optionsCol
            anchors.centerIn: parent
            spacing: 0.1 * (parent.height)
            Row {
                GrowingText {
                    text: qsTr("Fullscreen:", "Fullscreen: in options")
                    standardSize: 20
                    visible: os !== "sailfish"
                    anchors.verticalCenter: parent.verticalCenter
                    width: mainWindow.width * 0.2
                    color: "white"
                }
                MorzynCheckBox {
                    id: switchFullscreen
                    visible: os !== "sailfish"
                    checked: gameService.getBoolSetting("fullscreen")
                    onCheckedChanged: {
                        if (os === "sailfish") return;
                        if (os === "android") return;
                        allViews.fullscreen = checked;
                        console.log(allViews.fullscreen);
                        gameService.setFullScreen(allViews.fullscreen);
                    }
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Row {
                GrowingText {
                    text: qsTr("Music:", "Music: in options")
                    standardSize: 20
                    anchors.verticalCenter: parent.verticalCenter
                    width: mainWindow.width * 0.2
                    color: "white"
                }
                MorzynCheckBox {
                    id: switchMusic
                    checked: mainWindow.musicActivated
                    onCheckedChanged: {
                        mainWindow.musicActivated = checked;
                        gameService.setBoolSetting("music", mainWindow.musicActivated);
                    }
                }
            }
            Row {
                GrowingText {
                    text: qsTr("Sound:", "Sound: in options")
                    standardSize: 20
                    anchors.verticalCenter: parent.verticalCenter
                    width: mainWindow.width * 0.2
                    color: "white"
                }
                MorzynCheckBox {
                    id: switchSound
                    checked: mainWindow.soundActivated
                    onCheckedChanged: {
                        mainWindow.soundActivated = checked;
                        gameService.setBoolSetting("sound", mainWindow.soundActivated);
                    }
                }
            }
        }
    }
    MorzynButton {
        text: qsTr("Back", "Back in Options")
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20 * mainWindow.height / mainWindow.sourceHeight
        onClicked: {
            game.state = "mainMenuState";
        }
    }
}
