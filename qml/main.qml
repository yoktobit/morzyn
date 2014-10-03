// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.2
import harbour.morzyn 1.0
import QtMultimedia 5.0
import QtQuick.Controls 1.1

ApplicationWindow {
    visible: true
    visibility: game.fullScreen ? "FullScreen" : "Windowed"
    width: 800
    height: 600
    id: mainWindow

    /*minimumWidth: 600
    minimumHeight: width * 0.55
    maximumHeight: width * 0.76*/

    title: "Morzyn " + version

    property bool firstInitSound: true

    onVisibilityChanged: {
        console.debug("visibility: " + visibility);
        if ((visibility === 5 || visibility === 2 || visibility === 4)
                && (!firstInitSound)
                && (game.state !== "titleScreenState"))
        {
            gameService.playTitleSong(true);
        }
        else
        {
            gameService.playTitleSong(false);
        }
        firstInitSound === false;
    }

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
        asynchronous: true
        visible: status == Loader.Ready
    }

    TitleView {
        visible: game.state === "titleScreenState"
        tempLoader: loaderViews
    }

    /*MorzynAudio {
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
    }*/

    Component.onCompleted: {
        library.loadAll();
    }

}
