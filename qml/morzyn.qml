// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
import QtMultimedia 5.0
import Sailfish.Silica 1.0

ApplicationWindow {
    id: mainWindow
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    // rumdrehen...Sailfish...tztztz
    property double myWidth: height
    property double myHeight: width

    property double sourceWidth: 693
    property double sourceHeight: 499

    property bool playSounds: true
    property bool titleSoundLastPlayState: false

    property bool soundActivated: gameService.getBoolSetting("sound")
    property bool musicActivated: gameService.getBoolSetting("music")

    initialPage:
        Component { MorzynPage {} }
    Component.onCompleted: {
        library.loadAll();
    }
}
