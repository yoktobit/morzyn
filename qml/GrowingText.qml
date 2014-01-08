// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1

Text {
    property int standardSize: 14
    font.pixelSize: (standardSize * mainWindow.myHeight) / mainWindow.sourceHeight
    height: font.pixelSize + 2
}
