import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0

Page {
    id: morzynPage
    allowedOrientations: Qt.LandscapeOrientation
    SilicaGridView {
        anchors.fill: parent
        id: mainWindow

        property int sourceWidth: 693
        property int sourceHeight: 499

        Loader {
            anchors.fill: parent
            source: "Views.qml"
            asynchronous: true
            visible: status == Loader.Ready
        }

        TitleView {
            visible: game.state === "titleScreenState"
        }

        Audio {
            id: titleSound
            source: "sounds/morzyn intro.mp3"
            autoPlay: true
            loops: Audio.Infinite
            Behavior on volume {
                PropertyAnimation {
                    duration: 2000
                    onRunningChanged: {
                        if (!running)
                        {
                            if (titleSound.volume === 0)
                            {
                                titleSound.pause();
                            }
                        }
                    }
                }
            }
        }
    }
}
