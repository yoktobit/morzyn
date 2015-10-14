import QtQuick 2.0

Image {
    id: creatureHealingHeart

    property alias healingAmount: creatureHealingHeartText.text

    function start()
    {
        creatureHealingHeartAnimation.start();
    }

    source: "images/heart.png"
    anchors.fill: parent
    scale: 0
    Text {
        id: creatureHealingHeartText
        anchors.centerIn: parent
        font.pixelSize: (12 * mainWindow1.myHeight) / mainWindow.sourceHeight
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
                game.aquire();
            }
        }
    }
}
