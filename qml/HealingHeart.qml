import QtQuick 2.0

Image {
    id: creatureHealingHeart
    source: "images/heart.png"
    width: creatureImage.width
    height: creatureImage.height
    scale: 0
    Text {
        id: creatureHealingHeartText
        anchors.centerIn: parent
        font.pixelSize: (12 * mainWindow.myHeight) / mainWindow.sourceHeight
    }
    SequentialAnimation on scale {
        id: creatureHealingHeartAnimation
        alwaysRunToEnd: true
        running: false
        NumberAnimation {
            from: 0
            to: 1
            duration: 300
        }
        PauseAnimation {
            duration: 200
        }
        NumberAnimation {
            from: 1
            to: 0
            duration: 300
        }
        onRunningChanged: {
            if (!running)
            {
                console.log("State " + creatureImageRect.state);
                game.release();
            }
            else
            {

            }
        }
    }
}
