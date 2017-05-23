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
        source: "qrc:/images/images/game-over.gif"
        width: parent.parent.width * 0.7
        anchors.top: parent.top
        anchors.topMargin: Tile.tile_size * 1.5
        anchors.horizontalCenter: parent.horizontalCenter
    }

    CustomButton
    {
        anchors.centerIn: parent
        button_bgrd_color: "black"
        button_text: "go back"
        button_font: "Astron Boy"
        button_font_color: "lemonchiffon"
        button_italic: true
        button_font_size: 40
        onClicked: parent.clicked()
    }
}
