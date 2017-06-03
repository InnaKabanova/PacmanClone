import QtQuick 2.7
import "."

Item
{
    // When program starts, this item is visible by default:
    visible: true
    enabled: true
    anchors.fill: parent
    signal clicked

    Image
    {
        source: "qrc:/images/images/logo.png"
        width: parent.parent.width * 0.7
        anchors.top: parent.top
        anchors.topMargin: Tile.tile_size * 1.5
        anchors.horizontalCenter: parent.horizontalCenter
    }

    CustomButton
    {
        anchors.centerIn: parent
        button_bgrd_color: "black"
        button_text: "play game"
        button_font: "Helvetica"
        button_font_color: "lemonchiffon"
        button_font_size: 40
        onClicked: parent.clicked()
    }
}



