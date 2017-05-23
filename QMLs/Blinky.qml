import QtQuick 2.0
import "."

Image
{
    // Properties constantly updated from C++:
    // visible: false
    visible: true
    x: 0
    y: 0
    property int direction: 0

    width: Tile.tile_size
    height: Tile.tile_size
    clip: true

    Image
    {
        id: blinky_image
        source: "qrc:/images/images/ghost-blinky.png"
        anchors.left: parent.left
        anchors.leftMargin: -140
    }

    onDirectionChanged:
    {
        blinky_image.anchors.leftMargin = direction
    }

    onXChanged:
    {
        console.log("x:");
        console.log(x / 28);
    }

    onYChanged:
    {
        console.log("y:");
        console.log(y / 28);
    }

}


