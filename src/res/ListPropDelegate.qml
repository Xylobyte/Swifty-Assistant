import QtQuick 2.15

Rectangle {
    color: "steelblue"
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
            loading.running = true
            send.visible = false
            listMessage.model.insert(0, {
                                         "isSendUser": true,
                                         "text": model.text
                                     })
            swifty.messageSended(model.text)
            message.clear()
            swifty.newText("")
        }
    }
}
