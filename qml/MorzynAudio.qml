import QtQuick 2.0
import QtMultimedia 5.0

Audio {
    property bool isBehaviourEnabled: false
    function morzynPlay() {
        if (mainWindow.applicationActive && mainWindow.musicActivated)
            play();
    }
}
