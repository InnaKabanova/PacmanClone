import QtQuick 2.0

Rectangle
{
    id: button_rectangle
    width: 220
    height: 50

    // Revealing some of this component's properties
    // so it can be customized & re-used:
    property alias button_width: button_rectangle.width
    property alias button_height: button_rectangle.height
    property alias button_bgrd_color: button_rectangle.color
    property alias button_text: button_text.text
    property alias button_font: button_text.font.family
    property alias button_font_color: button_text.color
    property alias button_font_size: button_text.font.pointSize
    property alias button_italic: button_text.font.italic

    signal clicked

    Text
    {
        id: button_text
        text: "Button"
        anchors.centerIn: parent
        color: "black"
        font.family: "Helvetica"
        font.italic: false
        font.pointSize: 30
    }

    // For text resizing when hovering on it:
    property int original
    property int bigger: button_text.font.pointSize * 1.5

    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        onClicked:
        {
            button_rectangle.clicked()
        }
        onEntered:
        {
            original = button_text.font.pointSize
            button_text.font.pointSize = bigger
            button_text.opacity = 0.7
        }
        onExited:
        {
            button_text.font.pointSize = original
            button_text.opacity = 1
        }
    }
}
