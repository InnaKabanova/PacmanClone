import QtQuick 2.7
import "."

Column {
    visible: false
    spacing: 5

    Row
    {
        Text {
            text: "score:"
            font.family: "Helvetica"
            font.pointSize: 18
            color: "black"
        }

        Text {
            id: score_value
            text: current_game.get_score()
            font.family: "Helvetica"
            font.pointSize: 18
            color: "darkblue"
        }
    }

    Row{

        Image {
            source: "qrc:/images/images/health.png"
            width: Tile.tile_size
            height: Tile.tile_size
        }

        Text {
            id: health_value
            text: current_game.get_health()
            font.family: "Helvetica"
            font.pointSize: 18
            color: "darkblue"
        }
    }

    Row
    {
        Text {
            text: "level:"
            font.family: "Helvetica"
            font.pointSize: 18
            color: "black"
        }

        Text {
            id: level_value
            text: current_game.get_level()
            font.family: "Helvetica"
            font.pointSize: 18
            color: "darkblue"
        }
    }

    Connections
    {
        target: current_game

        onUpdateHealth:
        {
            health_value.text = new_health
        }

        onUpdateScore:
        {
            score_value.text = new_score
        }

        onUpdateLevel:
        {
            level_value.text = new_level
        }
    }
}

