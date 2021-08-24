import QtQuick 2.15

Rectangle {
    border.color: "steelblue"
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
            console.log("ok mouse area / "+model.action)
            swift.execAction(model.action)
        }
    }
}
