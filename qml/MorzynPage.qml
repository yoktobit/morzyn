import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0

Page {
    id: mainWindow1

    onWidthChanged: {
        console.log("Width:" + mainWindow1.myWidth);
    }
    onHeightChanged: {
        console.log("Height: " + mainWindow1.myHeight);
    }

    property double myWidth: width
    property double myHeight: height

    allowedOrientations: Orientation.Landscape

    SilicaGridView {
        anchors.fill: parent

        Loader {
            id: viewViews
            anchors.fill: parent
            source: "Views.qml"
            asynchronous: true
            visible: status == Loader.Ready
            /*onStatusChanged: {
                if (game.state === "titleScreenState" && status === Loader.Ready)
                {
                    game.state = "mainMenuState";
                }
            }*/
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
                    /*if (titleSound.playbackState === Audio.PlayingState)
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
                    console.log("TitleSound stopped");*/
                    gameService.playTitleSong(false);
                }
                else
                {
                    mainWindow.playSounds = true;
                    /*if (mainWindow.titleSoundLastPlayState)
                    {
                        titleSound.morzynPlay();
                        if (os !== "sailfish_sick")
                            titleSound.volume = 1.0;
                        console.log("TitleSound resumed");
                    }*/
                    gameService.playTitleSong(true);
                }
            }
        }

        /*MorzynAudio {
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
        }*/
    }
}
