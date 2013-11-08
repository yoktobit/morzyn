import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Image {
        id: label
        width: parent.width
        height: parent.width
        antialiasing: false
        smooth: false
        anchors.centerIn: parent
        source: "../images/dark priest red.png"
    }
    
    /*CoverActionList {
        id: coverAction
        
        CoverAction {
            iconSource: "image://theme/icon-cover-next"
        }
        
        CoverAction {
            iconSource: "image://theme/icon-cover-pause"
        }
    }*/
}


