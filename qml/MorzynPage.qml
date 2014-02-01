import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0

Page {
    id: mainWindow1
    allowedOrientations: Qt.LandscapeOrientation

    onWidthChanged: {
        console.log("Width:" + mainWindow.myWidth);
    }
    onHeightChanged: {
        console.log("Height: " + mainWindow.myHeight);
    }

    SilicaGridView {
        anchors.fill: parent

        Loader {
            id: viewViews
            anchors.fill: parent
            source: "Views.qml"
            asynchronous: true
            visible: status == Loader.Ready
            onStatusChanged: {
                if (game.state === "titleScreenState" && status === Loader.Ready)
                {
                    game.state = "mainMenuState";
                }
            }
        }

        TitleView {
            visible: game.state === "titleScreenState" || viewViews.status !== Loader.Ready
            tempLoader: viewViews
        }

        Connections {
            target: mainWindow
            onApplicationActiveChanged: {
                if (!mainWindow.applicationActive)
                {
                    mainWindow.playSounds = false;
                    if (titleSound.playbackState === Audio.PlayingState)
                    {
                        mainWindow.titleSoundLastPlayState = true;
                        console.log("TitleSound was running");
                    }
                    else
                    {
                        mainWindow.titleSoundLastPlayState = false;
                        console.log("TitleSound was not running");
                    }
                    if (os === "sailfish_sick")
                        titleSound.pause();
                    else
                        titleSound.volume = 0.0;
                    console.log("TitleSound stopped");
                }
                else
                {
                    mainWindow.playSounds = true;
                    if (mainWindow.titleSoundLastPlayState)
                    {
                        titleSound.morzynPlay();
                        if (os !== "sailfish_sick")
                            titleSound.volume = 1.0;
                        console.log("TitleSound resumed");
                    }
                }
            }
        }

        MorzynAudio {
            id: titleSound
            source: "sounds/morzyn intro.mp3"
            autoPlay: false
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
