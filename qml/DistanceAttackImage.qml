// Copyright 2013 by Martin Windolph
// Do not remove copyright notice

import QtQuick 2.1
import QtMultimedia 5.0

Image {
    id: distanceAttackImage
    property var attackingCreature: null
    property var attackedCreature: null
    property int nDamage
    visible: false
    signal animationFinished(var attackedCreature, int nDamage)

    Audio {
        id: distFire01
        source: "sounds/distfire01.wav"
        loops: 0
    }
    Audio {
        id: distFire02
        source: "sounds/distfire02.wav"
    }
    Audio {
        id: distFire03
        source: "sounds/distfire03.wav"
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
    Audio {
        id: distHit01
        source: "sounds/disthit01.wav"
        loops: 0
    }
    Audio {
        id: distHit02
        source: "sounds/disthit02.wav"
        loops: 0
    }
    Audio {
        id: distHit03
        source: "sounds/disthit03.wav"
        loops: 0
    }
    Audio {
        id: distMiss01
        source: "sounds/distmiss01.wav"
    }
    Audio {
        id: distMiss02
        source: "sounds/distmiss02.wav"
    }
    Audio {
        id: distMiss03
        source: "sounds/distmiss03.wav"
    }

    function playOneOf(sound1, sound2, sound3)
    {
        if (!mainWindow.soundActivated || !mainWindow.playSounds || os === "sailfish_sick") return;
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

    function calculateRotation(creature1, creature2)
    {
        if (creature1 === null || creature2 === null) return 0;
        var pt1X = creature1.x;
        var pt1Y = creature1.y;
        var pt2X = creature2.x;
        var pt2Y = creature2.y;
        var ptDiffX = pt1X - pt2X;
        var ptDiffY = pt1Y - pt2Y;
        var ret = (Math.atan2(ptDiffY, ptDiffX) * 180) / Math.PI
        console.log("Rotation of Distance Image:" + ret);
        return ret;
    }
    x: attackingCreature ? attackingCreature.xField * (((416.0 / hCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) : 0 // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
    y: attackingCreature ? attackingCreature.yField * (((416.0 / vCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) : 0
    z: 5000
    height: ((416.0 / vCount) * mainWindow1.myHeight) / mainWindow.sourceHeight // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
    width: height
    source: attackingCreature ? "images/" + attackingCreature.distanceImageFilename : ""
    rotation: calculateRotation(attackingCreature, attackedCreature)

    function getDuration(x1, x2, y1, y2)
    {
        var xDiff = x2 - x1;
        var yDiff = y2 - y1;
        var duration = Math.sqrt(Math.pow(xDiff, 2) + Math.pow(yDiff, 2)) * 500.0;
        console.log("Duration: " + duration);
        return duration;
    }

    ParallelAnimation {
        id: distanceAttackImageAnimation
        running: false
        NumberAnimation {
            id: distanceAttackImageAnimationX
            target: distanceAttackImage
            property: "x"
            from: attackingCreature ? attackingCreature.xField * (((416.0 / hCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) : 0 // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
            to: attackedCreature ? attackedCreature.xField * (((416.0 / vCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) : 0 // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
            duration: getDuration(attackingCreature ? attackingCreature.xField : 0, attackedCreature ? attackedCreature.xField : 0, attackingCreature ? attackingCreature.yField : 0, attackedCreature ? attackedCreature.yField : 0);
            running: false
        }

        NumberAnimation {
            id: distanceAttackImageAnimationY
            target: distanceAttackImage
            property: "y"
            from: attackingCreature ? attackingCreature.yField * (((416.0 / hCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) : 0 // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
            to: attackedCreature ? attackedCreature.yField * (((416.0 / vCount) * mainWindow1.myHeight) / mainWindow.sourceHeight) : 0 // Absicht, damit width immer = height, damit Seitenverhältnis bleibt
            duration: getDuration(attackingCreature ? attackingCreature.xField : 0, attackedCreature ? attackedCreature.xField : 0, attackingCreature ? attackingCreature.yField : 0, attackedCreature ? attackedCreature.yField : 0);
            running: false
        }
        onRunningChanged: {
            if (!running)
            {
                animationFinished(attackedCreature, nDamage);
                distFly01.stop(); distFly02.stop(); distFly03.stop();
                var immune = false;
                if (attackedCreature.immune === "ALL")
                {
                    immune = true;
                }
                else if (attackingCreature.immune && attackedCreature.immune && attackingCreature.immune === attackedCreature.immune)
                {
                    immune = true;
                }

                if (nDamage > 0 && !immune)
                {
                    playOneOf(distHit01, distHit02, distHit03);
                }
                else
                {
                    playOneOf(distMiss01, distMiss02, distMiss03);
                }
                distanceAttackImageRotation.stop();
                visible = false;

            }
        }
    }

    NumberAnimation on rotation {
        id: distanceAttackImageRotation
        from: 0
        to: 360
        duration: 800
        running: false
        loops: Animation.Infinite
    }

    function start()
    {
        console.log("x:" + distanceAttackImageAnimationX.from);
        console.log("y:" + distanceAttackImageAnimationY.from);
        console.log("width:" + width);
        console.log("height:" + height);
        console.log("toX: " + distanceAttackImageAnimationX.to);
        console.log("toY: " + distanceAttackImageAnimationY.to);
        visible = true;
        distanceAttackImageAnimation.start();
        if (os !== "sailfish_sick") {
            playOneOf(distFire01, distFire02, distFire03);
            playOneOf(distFly01, distFly02, distFly03);
        }
        if (attackingCreature.distanceImageFilename.indexOf("arrow") < 0)
        {
            distanceAttackImageRotation.start();
        }
        game.aquire();
    }
}
