import QtQuick 2.15

Rectangle {
    id: rect
    border.color: "#bf99a5"
    border.width: 2
    color: "#00ffffff"
    radius: 20
    height: 25
    width: text.implicitWidth+20

    Text {
        id: text
        anchors.centerIn: parent
        text: model.text
        font.pointSize: 8
        color: "white"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            anim.running = true
            swifty.execAction(model.action)
        }
    }

    NumberAnimation {
        id: anim
        target: rect
        duration: 500
        from: 0.2
        to: 1.0
        property: "opacity"
    }
}
