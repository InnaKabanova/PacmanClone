import QtQuick 2.7
import "."

Item
{
    // When program starts, this item is invisible by default:
    visible: false
    enabled: false
    anchors.fill: parent
    signal clicked

    AnimatedImage
    {
        id: result_gif
        visible: true
        source: "qrc:/images/images/game-over.gif"

        anchors.top: parent.top
        anchors.topMargin: Tile.tile_size * 1.5
        anchors.horizontalCenter: parent.horizontalCenter

        width: parent.parent.width * 0.5
    }

    Text
    {
        id: result_text

        anchors.top: result_gif.bottom
        anchors.topMargin: Tile.tile_size * 1.5
        anchors.horizontalCenter: result_gif.horizontalCenter

        text: "test"
        font.family: "Astron Boy"
        font.pointSize: 40
        font.italic: true
        color: "lemonchiffon"
    }

    CustomButton
    {
        anchors.top: result_text.bottom
        anchors.topMargin: Tile.tile_size * 1.5
        anchors.horizontalCenter: result_text.horizontalCenter

        button_bgrd_color: "black"
        button_text: "go back"
        button_font: "Astron Boy"
        button_font_color: "lemonchiffon"
        button_italic: true
        button_font_size: 40

        onClicked: parent.clicked()
    }

    Connections
    {
        target: current_game
        onPassed:
        {
            if(victory === true)
            {
                result_text.text = "You won!"
                result_gif.source = "qrc:/images/images/game-over.gif"
            }
            else if(victory === false)
            {
                result_text.text = "You failed..."
                result_gif.source = "qrc:/images/images/game-over-2.gif"
            }
        }
    }
}
