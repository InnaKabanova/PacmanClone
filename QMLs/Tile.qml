pragma Singleton
import QtQuick 2.0

QtObject
{
    property int tile_size: current_game.get_tilesize()
}
