import QtQuick 2.7
import QtQuick.Window 2.2
import "." // referencing the Singleton object ('Tile.qml')

Window
{
    title: qsTr("Pacman Clone")
    visible: true

    width: Tile.tile_size * 19
    height: Tile.tile_size * 25
    // Disable scaling:
    maximumWidth: width
    maximumHeight: height
    minimumWidth: width
    minimumHeight: height

    Image
    {
        source: "qrc:/images/images/background.png"
    }

    NewGameScreen
    {
        id: newgame
        onClicked: // If 'play game' button is clicked:
        {
            // 1. hide the 'NewGameScreen' item which fills the window,
            // disable receiving of mouse events:
            visible = false
            enabled = false
            // 2. make the 'GameField' item visible and enable receiving of
            // keyboard events:
            gamefield.visible = true
            gamefield.enabled = true
            infobar.visible = true
            // 3. call the C++ method to start the game from the first
            // level and dynamically create the QML representation of
            // the game objects:
            current_game.start_level()
        }
    }

    GameField
    {
        id: gamefield
    }

    InfoBar
    {
        id: infobar
        anchors.bottom: gamefield.bottom
    }

    Connections // If player has passed all the levels or the game is over:
    {
        // ('void passed()' signal was emitted by 'CurrentGame' object)
        target: current_game
        onPassed:
        {
            // 1. hide the 'GameField' item which fills the window,
            // disable receiving of keyboard events:
            gamefield.visible = false
            gamefield.enabled = false
            infobar.visible = false
            // 2. make the 'GameOverScreen' item visible and enable
            // receiving of mouse events:
            gameover.visible = true
            gameover.enabled = true
        }
    }

    GameOverScreen
    {
        id: gameover
        onClicked: // If 'go back' button is clicked:
        {
            // 1. hide the 'GameOverScreen' item which fills the window,
            // disable receiving of mouse events:
            visible = false
            enabled = false
            // 2. make the 'NewGameScreen' item visible and enable
            // receiving of mouse events:
            newgame.visible = true
            newgame.enabled = true
            infobar.visible = false
        }
    }
}















