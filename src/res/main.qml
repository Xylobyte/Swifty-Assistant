import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.12
import Qt.labs.settings 1.1

import SwiftWorker 1.0

Window {
    id: window
    width: 510  
    height: 700
    visible: false
    flags: Qt.FramelessWindowHint
    color: "transparent"
    title: qsTr("Worker Assistant")

    onActiveChanged: {
        if (active === false) window.visible = false
    }

    property QtObject webView: WebEngineView {}
    property QtObject settingsView: SettingsView {}
    property QtObject customView: CustomQmlView {}

    property string type
    property var site

    Rectangle {
        x: 10
        y: 10
        color: "#171717"
        width: parent.width-20
        height: parent.height-20
        radius: 15

        Settings {
            id: settings
        }

        Timer {
            id: timerWeb
            interval: 1600
            repeat: false
            running: false
            onTriggered: {
                stack.push(webView, {"typeWeb": type, "webUrl": site})
            }
        }

        Timer {
            id: timerSettings
            interval: 1600
            repeat: false
            running: false
            onTriggered: {
                stack.push(settingsView)
            }
        }

        Timer {
            id: timerHide
            interval: 1600
            repeat: false
            running: false
            onTriggered: {
                window.visible = false
            }
        }

        Connections {
            target: swift

            function onReponse(text, isFin, typeMessage, url, textUrl) {
                if (typeMessage === "web_without_action_btn" || typeMessage === "web_with_action_btn") {
                    type = typeMessage
                    site = url
                    timerWeb.running = true
                }

                else if (typeMessage === "settings") {
                    type = typeMessage
                    timerSettings.running = true
                }
            }

            function onShowWindow(x, y) {
                window.visible = true
                window.x = x-(window.width/2)
                window.y = y-(window.height/2)
            }

            function onHideWindow() {
                timerHide.running = true
            }

            function onShowQml(fileUrl) {
                stack.push(customView, {"qmlUrl": fileUrl})
            }
        }

        Swift {
            id: swift
        }

        StackView {
            id: stack
            initialItem: mainPage
            anchors.fill: parent
            clip: true

            MainPage {
                id: mainPage
            }
        }
    }
}
