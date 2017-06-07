import QtQuick 2.7

Item
{
    // When program starts, this item is invisible by default:
    visible: false
    enabled: false
    anchors.fill: parent

    Item {
        id: keys_detector
        focus: true

        Keys.onPressed:
        {
           if (event.key === Qt.Key_Left)
           {
               pacman_object.move(180);
               event.accepted = true;
           }
           else if(event.key === Qt.Key_Right)
           {
               pacman_object.move(0);
               event.accepted = true;
           }
           else if(event.key === Qt.Key_Up)
           {
               pacman_object.move(270);
               event.accepted = true;
           }
           else if(event.key === Qt.Key_Down)
           {
               pacman_object.move(90);
               event.accepted = true;
           }
//           else if(event.key === Qt.Key_Space) // For debug purposes
//           {
//               current_game.start_level();
//               // current_game.try_again();
//               event.accepted = true;
//           }
        }
    }
}
