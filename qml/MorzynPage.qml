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

        property bool playSounds: true
        property bool titleSoundLastPlayState: false

        Loader {
            anchors.fill: parent
            source: "Views.qml"
            asynchronous: true
            visible: status == Loader.Ready
        }

        TitleView {
            visible: game.state === "titleScreenState"
        }

        Connections {
            target: morzynApp
            onApplicationActiveChanged: {
                if (!morzynApp.applicationActive)
                {
                    mainWindow.playSounds = false;
                    if (titleSound.playbackState == Audio.PlayingState)
                    {
                        mainWindow.titleSoundLastPlayState = true;
                        console.log("TitleSound was running");
                    }
                    else
                    {
                        mainWindow.titleSoundLastPlayState = false;
                        console.log("TitleSound was not running");
                    }

                    titleSound.pause();
                    console.log("TitleSound stopped");
                }
                else
                {
                    mainWindow.playSounds = true;
                    if (mainWindow.titleSoundLastPlayState)
                    {
                        titleSound.play();
                        console.log("TitleSound resumed");
                    }
                }
            }
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
