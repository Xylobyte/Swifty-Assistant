import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick 2.15

Rectangle {
    anchors.fill: parent
    color: "#171717"
    radius: 15

    Connections {
        id: connect
        target: swifty

        function onReponse(text, isFin, typeMessage, url, textUrl) {
            txtName.text = settings.value("settings_name", "")
            checkProp.checked = settings.value("settings_proposition", "true") === "true" ? true : false
        }

        function onPluginName(name) {
            listPlugin.model.append({"text": name})
        }
    }

    Component.onCompleted: {
        txtName.text = settings.value("settings_name")
        checkProp.checked = settings.value("settings_proposition", "true") === "true" ? true : false
        swifty.getPluginList()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 10

        Text {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 20
            font.bold: true
            color: "white"
            text: qsTr("Paramètres")
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 10

            Text {
                Layout.rightMargin: 20
                text: qsTr("Votre nom:")
                font.pointSize: 13
                color: "white"
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.leftMargin: 40
                implicitHeight: 40
                color: "transparent"
                border.color: "#ffc9b3"
                radius: 8

                TextField {
                    id: txtName
                    focus: true
                    font.pointSize: 10
                    selectByMouse: true
                    maximumLength: 50
                    anchors.fill: parent
                    anchors.margins: 6
                    placeholderText: qsTr("Entrez votre nom")
                    background: Rectangle {
                        color: "transparent"
                    }
                    color: "white"

                    onAccepted: settings.setValue("settings_name", text)
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 10

            Text {
                Layout.rightMargin: 20
                text: qsTr("Suggestion:")
                font.pointSize: 13
                color: "white"
            }

            CheckBox {
                id: checkProp
                indicator: Rectangle {
                    implicitWidth: 26
                    implicitHeight: 26
                    x: checkProp.leftPadding
                    y: parent.height / 2 - height / 2
                    radius: 4
                    color: "transparent"
                    border.color: checkProp.down ? "#95799c" : "#ffc9b3"
                    border.width: 2

                    Rectangle {
                        width: 14
                        height: 14
                        x: 6
                        y: 6
                        radius: 3
                        color: checkProp.down ? "#95799c" : "#ffc9b3"
                        visible: checkProp.checked
                    }
                }

                onClicked: {
                    settings.setValue("settings_proposition", checked)
                    anim.running = true
                }

                NumberAnimation {
                    id: anim
                    target: checkProp
                    duration: 500
                    from: 0.2
                    to: checkProp.opacity
                    property: "opacity"
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 10

            Text {
                Layout.rightMargin: 20
                text: qsTr("Plugins:")
                font.pointSize: 13
                color: "white"
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Rectangle {
            border.color: "#aa89a0"
            border.width: 2
            radius: 5
            color: "#171717"
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: listPlugin
                clip: true
                anchors.fill: parent
                anchors.margins: 5
                maximumFlickVelocity: 1300
                spacing: 10
                model: ListModel {}
                delegate: RowLayout {
                    width: parent.width
                    Text {
                        id: txtDelegate
                        text: model.text
                        color: "white"
                        font.pointSize: 14
                        Layout.fillWidth: true
                    }
                }

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
            }
        }

        RowLayout {
            Layout.fillWidth: true

            MButton {
                text: qsTr("Accueil")
                font.pointSize: 8
                implicitHeight: 21
                borderWidth: 2
                radius: 10
                onClicked: {
                    settings.setValue("settings_name", txtName.text)
                    swifty.execAction("app home")
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MButton {
                text: qsTr("A propos")
                font.pointSize: 8
                implicitHeight: 21
                borderWidth: 2
                radius: 10
                onClicked: {
                    aboutDialog.open()
                }
            }
        }

        AboutDialog {
            id: aboutDialog
        }
    }
}
