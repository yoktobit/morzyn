import QtQuick 2.0
import QtMultimedia 5.0

Item {
    id: manaTransferImage
    height: (416.0 / vCount) * mainWindow1.myHeight / mainWindow.sourceHeight // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
    width: (416.0 / vCount) * mainWindow1.myHeight / mainWindow.sourceHeight

    property var attackingCreature: null
    property var attackedCreature: null

    Image {
        source: "images/manatransfer.png"
        anchors.fill: parent
    }

    Audio {
        id: distFly01
        source: "sounds/distfly01.wav"
    }
    Audio {
        id: distFly02
        source: "sounds/distfly02.wav"
    }
    Audio {
        id: distFly03
        source: "sounds/distfly03.wav"
    }

    function playOneOf(sound1, sound2, sound3)
    {
        if (!mainWindow.soundActivated || !mainWindow.playSounds) return;
        var randomnumber = Math.floor(Math.random() * 3);
        console.log("DistFire " + randomnumber);
        switch (randomnumber)
        {
        case 0:
            sound1.play();
            break;
        case 1:
            sound2.play();
            break;
        case 2:
            sound3.play();
            break;
        }
    }

    function getDuration(x1, x2, y1, y2)
    {
        var xDiff = x2 - x1;
        var yDiff = y2 - y1;
        var duration = Math.sqrt(Math.pow(xDiff, 2) + Math.pow(yDiff, 2)) * 500.0;
        return duration;
    }

    ParallelAnimation {
        id: manaTransferImageAnimation
        running: false
        alwaysRunToEnd: true
        NumberAnimation {
            id: manaTransferImageAnimationX
            target: manaTransferImage
            property: "x"
            from: attackingCreature ? attackingCreature.xField * (((416.0 / hCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) : 0 // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
            to: attackedCreature ? attackedCreature.xField * (((416.0 / vCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) : 0 // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
            duration: getDuration(attackingCreature ? attackingCreature.xField : 0, attackedCreature ? attackedCreature.xField : 0, attackingCreature ? attackingCreature.yField : 0, attackedCreature ? attackedCreature.yField : 0);
        }

        NumberAnimation {
            id: manaTransferImageAnimationY
            target: manaTransferImage
            property: "y"
            from: attackingCreature ? attackingCreature.yField * (((416.0 / hCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) : 0 // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
            to: attackedCreature ? attackedCreature.yField * (((416.0 / vCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) : 0 // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
            duration: getDuration(attackingCreature ? attackingCreature.xField : 0, attackedCreature ? attackedCreature.xField : 0, attackingCreature ? attackingCreature.yField : 0, attackedCreature ? attackedCreature.yField : 0);
        }
        onRunningChanged: {
            if (!running)
            {
                distFly01.stop(); distFly02.stop(); distFly03.stop();
                manaTransferImage.visible = false;
                game.release();
            }
            else
            {
                manaTransferImage.visible = true;
                game.aquire();
            }
        }
    }

    NumberAnimation on rotation {
        id: manaTransferImageRotation
        from: 0
        to: 360
        duration: 800
        running: false
        loops: Animation.Infinite
    }

    function start()
    {
        manaTransferImage.visible = true;
        manaTransferImageAnimation.start();
        manaTransferImageRotation.start();

        playOneOf(distFly01, distFly02, distFly03);
    }
}
