import QtQuick 2.0

Rectangle {
    width: 0.04 * mainWindow.myWidth
    height: width
    Image {
        id: menuImage
        source: "images/morzynmenu.png"
        anchors.centerIn: parent
        width: 0.03 * mainWindow.myWidth
        height: width
    }
    color: "#44FFFFFF"
    radius: width / 5.0
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (game.hasBegun)
            {
                console.debug("calling in-game-menu");
                inGameMenuView.visible = true;
            }
            else
            {
                console.debug("directly back to menu");
                gameService.resetGame();
                game.state = "mainMenuState";
            }
        }
    }
}
