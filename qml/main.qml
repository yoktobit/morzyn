// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
import harbour.morzyn 1.0
import QtMultimedia 5.0

Rectangle {

    width: 693
    height: 499
    id: mainWindow

    property int sourceWidth: 693
    property int sourceHeight: 499

    property double myWidth: width
    property double myHeight: height

    property bool playSounds: true

    property bool soundActivated: gameService.getBoolSetting("sound")
    property bool musicActivated: gameService.getBoolSetting("music")

    property bool applicationActive: true

    Loader {
        id: loaderViews
        anchors.fill: parent
        source: "Views.qml"
        focus: true
        asynchronous: true
        visible: status == Loader.Ready
    }

    TitleView {
        visible: game.state === "titleScreenState"
        tempLoader: loaderViews
    }

    MorzynAudio {
        id: titleSound
        source: "sounds/morzyn intro.mp3"
        autoPlay: false
        autoLoad: true
        loops: Audio.Infinite
        Behavior on volume {
            enabled: titleSound.isBehaviourEnabled
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

    Component.onCompleted: {
        library.loadAll();
    }

}
