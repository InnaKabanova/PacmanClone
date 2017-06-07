import QtQuick 2.7
import "."

Image
{
    // Properties constantly updated from C++:
    visible: false
    x: 0
    y: 0
    property int direction: 0

    width: Tile.tile_size
    height: Tile.tile_size
    clip: true

    Image
    {
        id: blinky_image
        source: "qrc:/images/images/ghost-inky.png"
        anchors.left: parent.left
        anchors.leftMargin: 0
    }

    onDirectionChanged:
    {
        blinky_image.anchors.leftMargin = direction
    }
}
