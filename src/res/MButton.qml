import QtQuick
import QtQuick.Controls

Button {
    id: button

    property int radius: 2
    property int borderWidth: 3

    property string primaryColor: "#ffc9b3"
    property string primaryColorlicked: "#95799c"

    contentItem: Text {
        text: button.text
        font: button.font
        opacity: enabled ? 1.0 : 0.3
        color: button.down ? primaryColorlicked : primaryColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        opacity: enabled ? 1 : 0.3
        color: "#00ffffff"
        border.color: button.down ? primaryColorlicked : primaryColor
        border.width: button.down ? 1 : borderWidth
        radius: button.radius
    }

    onClicked: anim.running = true

    NumberAnimation {
        id: anim
        target: button
        duration: 500
        from: 0.2
        to: button.opacity
        property: "opacity"
    }
}
