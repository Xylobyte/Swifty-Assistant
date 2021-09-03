import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    anchors.fill: parent
    anchors.margins: 10
    spacing: 15

    Connections {
        target: swift

        function onReponse(text, isFin, typeMessage, url, textUrl) {
            if (isFin)
                loading.running = false
            if (isFin)
                send.visible = true

            if (typeMessage === "message") {
                listMessage.model.insert(0, {"isSendUser": false, "text": text})

                listAction.model.clear()
                listAction.visible = false
                for(var i = 0; i < url.length; i++) {
                    listAction.visible = true
                    listAction.model.append({"text": textUrl[i], "action":url[i]})
                }
            }
        }

        function onAddProposition(prop) {
            listProp.model.append({"text": prop})
            txtNoEntries.visible = false
        }

        function onRemoveProposition(index) {
            listProp.model.remove(index)

            if (listProp.model.count < 1) txtNoEntries.visible = true
        }

        function onRemoveAllProposition() {
            listProp.model.clear()
            if (listProp.model.count < 1) txtNoEntries.visible = true
        }
    }

    Component.onCompleted: {
        var hello = qsTr("Bonjour ")
        listMessage.model.insert(0, {
                                     "isSendUser": false,
                                     "text": hello + settings.value("settings_name", "") + "!"
                                 })
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        color: "#171717"
        border.color: "#595959"
        border.width: 3
        radius: 15

        ColumnLayout {
            id: layoutMessage
            anchors.fill: parent
            anchors.margins: 10

            ListView {
                id: listMessage
                clip: true
                maximumFlickVelocity: 1300
                verticalLayoutDirection: ListView.BottomToTop
                displayMarginBeginning: 40
                displayMarginEnd: 40
                spacing: 10
                Layout.fillHeight: true
                Layout.fillWidth: true

                add: Transition {
                    NumberAnimation {
                        properties: "y"
                        from: 0
                        duration: 400
                    }
                }

                Keys.onUpPressed: scrollBar.decrease()
                Keys.onDownPressed: scrollBar.increase()
                ScrollBar.vertical: ScrollBar {
                    id: scrollBar

                    background: Rectangle {
                        color: "transparent"
                    }

                    contentItem: Rectangle {
                        implicitWidth: 6
                        implicitHeight: 100
                        radius: width / 3
                        color: scrollBar.pressed ? "white" : "#696969"
                    }
                }

                model: ListModel {
                    id: listMessageModel
                }
                delegate: ListMessageDelegate {}
            }

            ListView {
                id: listAction
                visible: false
                orientation: ListView.Horizontal
                Layout.fillWidth: true
                Layout.leftMargin: 40
                height: 25
                clip: true
                maximumFlickVelocity: 1300
                spacing: 10
                model: ListModel {
                    id: listActionModel
                }
                delegate: ListActionDelegate {}

                add: Transition {
                    NumberAnimation {
                        properties: "x"
                        from: 1000
                        duration: 300
                    }
                }

                remove: Transition {
                    NumberAnimation {
                        properties: "x"
                        from: 1000
                        duration: 300
                    }
                }
            }
        }
    }

    ListView {
        id: listProp
        visible: settings.value("settings_proposition", "true") === "true"
        orientation: ListView.Horizontal
        Layout.fillWidth: true
        Layout.leftMargin: 20
        Layout.rightMargin: 20
        height: 25
        clip: true
        maximumFlickVelocity: 1300
        spacing: 10
        model: ListModel {
            id: listPropModel
        }
        delegate: ListPropDelegate {}

        add: Transition {
            NumberAnimation {
                properties: "x"
                from: 1000
                duration: 400
            }
            NumberAnimation {
                properties: "opacity"
                from: 0
                duration: 400
            }
        }

        remove: Transition {
            NumberAnimation {
                properties: "y"
                to: 50
                duration: 400
            }
            NumberAnimation {
                properties: "opacity"
                to: 0
                duration: 400
            }
        }

        displaced: Transition {
            NumberAnimation { property: "x"; duration: 400 }
        }

        Text {
            id: txtNoEntries
            text: qsTr("Aucune suggestion !")
            color: "white"
            anchors.centerIn: parent
        }
    }

    RowLayout {
        spacing: 10

        BusyIndicator {
            id: loading
            running: false
            implicitWidth: 25
            implicitHeight: implicitWidth
            Layout.leftMargin: 10
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.leftMargin: 40
            implicitHeight: 40
            color: "transparent"
            border.color: "#8eb1fe"
            radius: 4

            TextField {
                id: message
                focus: true
                font.pointSize: 10
                selectByMouse: true
                maximumLength: 200
                anchors.fill: parent
                anchors.margins: 6
                placeholderText: qsTr("Entrez votre demande")
                background: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: 40
                    color: "transparent"
                }
                color: "white"

                onAccepted: {
                    if (text !== "" && loading.running === false) {
                        loading.running = true
                        send.visible = false
                        listMessage.model.insert(0, {
                                                     "isSendUser": true,
                                                     "text": listProp.model.count > 0 ? listProp.model.get(0).text : message.text
                                                 })
                        swift.messageSended(listProp.model.count > 0 ? listProp.model.get(0).text : message.text)
                        message.clear()
                        swift.newText("")
                    }
                }

                onTextEdited: {
                    time.restart()
                }
            }
        }

        Button {
            id: send
            icon.source: "qrc:/Icon/CommonIcon/send.svg"
            onClicked: {
                if (message.text !== "") {
                    loading.running = true
                    send.visible = false
                    listMessage.model.insert(0, {
                                                 "isSendUser": true,
                                                 "text": message.text
                                             })
                    swift.messageSended(message.text)
                    message.clear()
                    swift.newText("")
                }
            }
        }

        Button {
            id: btnSettings
            icon.source: "qrc:/Icon/CommonIcon/settings.svg"
            onClicked: {
                stack.push(settingsView)
            }
        }

        Timer {
            id: time
            repeat: false
            running: false
            interval: 300
            onTriggered: {
                swift.newText(message.text)
            }
        }
    }
}
