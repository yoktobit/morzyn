// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Rectangle {
    width: scrollImage.width
    height: scrollImage.height
    Image {
        id: scrollImage
        source: "images/" + imageFilename
        height: (((sourceSize.height * 13.0) / vCount) * mainWindow1.myHeight) / mainWindow.sourceHeight // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
        width: height
    }
}
