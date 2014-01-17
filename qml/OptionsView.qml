import QtQuick 2.0

Item {
    id: optionsView
    anchors.fill: parent
    Image {
        source: "images/titleback.png"
        anchors.fill: parent
    }

    Grid {
        anchors.centerIn: parent
        spacing: 0.1 * (parent.height)
        rows: 2
        columns: 2
        GrowingText {
            text: qsTr("Fullscreen:", "Fullscreen: in options")
            standardSize: 20
            visible: os !== "sailfish"
        }
        Switch {
            id: switchFullscreen
            visible: os !== "sailfish"
            model: ListModel {
                ListElement {
                    name: "X"
                    value: "true"
                }
                ListElement {
                    name: "-"
                    value: "false"
                }
            }
            onSelectedValueChanged: {
                allViews.fullscreen = !allViews.fullscreen;
                console.log(allViews.fullscreen);
                gameService.setFullScreen(fullscreen);
            }
        }
        GrowingText {
            text: qsTr("Music:", "Music: in options")
            standardSize: 20
        }
        Switch {
            id: switchMusic
            model: ListModel {
                ListElement {
                    name: "X"
                    value: "true"
                }
                ListElement {
                    name: "-"
                    value: "false"
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
