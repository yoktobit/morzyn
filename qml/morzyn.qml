// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
import QtMultimedia 5.0
import Sailfish.Silica 1.0

ApplicationWindow {
    id: morzynApp    
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    initialPage:
        MorzynPage {}
    Component.onCompleted: {
        library.loadAll();
    }
}
