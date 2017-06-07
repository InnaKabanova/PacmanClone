import QtQuick 2.7
import "."

AnimatedImage
{
    id: pacman_image
    source: "qrc:/images/images/pacman.gif"
    width: Tile.tile_size
    height: Tile.tile_size
    rotation: 0

    // Properties constantly updated from C++:
    visible: false // (initial value)
    x: pacman_object.get_x() * Tile.tile_size // (initial value)
    y: pacman_object.get_y() * Tile.tile_size // (initial value)
    property alias direction: pacman_image.rotation // (initial value is 0)
}





