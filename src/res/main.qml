import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import Qt.labs.settings 1.1
import QtQuick.Dialogs 1.3
import QtQuick.Particles 2.15
import QtQuick.XmlListModel 2.15
import QtQuick3D 1.15
import QtQuick.Extras 1.4
import QtQuick.Timeline 1.0
import QtQml 2.15

import SwiftyWorker 1.0

Window {
    id: window
    width: 510  
    height: 700
    visible: false
    color: "transparent"
    title: qsTr("Swifty Assistant")

    onActiveChanged: {
        if (active === false) window.visible = false
        swifty.setWindowVisibility(false)
    }

    property QtObject webView: WebEngineView {}
    property QtObject settingsView: SettingsView {}
    property QtObject customView: CustomQmlView {}

    property string type
    property string site

    Rectangle {
        x: swifty.getOs() !== "windows" ? 10 : 0
        y: swifty.getOs() !== "windows" ? 10 : 0
        color: "#171717"
        width: swifty.getOs() !== "windows" ? parent.width-20 : parent.width
        height: swifty.getOs() !== "windows" ? parent.height-20 : parent.height
        radius: 15

        Settings {
            id: settings
        }

        Timer {
            id: timerWeb
            interval: 1000
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

        Connections {
            target: swifty

            function onReponse(text, isFin, typeMessage, url, textUrl) {
                if (typeMessage === "web_without_action_btn" || typeMessage === "web_with_action_btn") {
                    type = typeMessage
                    site = url[0]
                    timerWeb.running = true
                }

                else if (typeMessage === "settings") {
                    type = typeMessage
                    timerSettings.running = true
                }
            }

            function onShowWindow(x, y) {
                if (swifty.getOs() !== "windows") {
                    window.flags = Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint
                    window.visible = true
                    window.x = x-(window.width/2)
                    window.y = y-(window.height/2)
                }
                else {
                    window.flags = Qt.WindowStaysOnTopHint | Qt.Popup
                    window.visible = true
                    window.y = Screen.desktopAvailableHeight - height - 10
                    window.x = Screen.desktopAvailableWidth - width - 10
                }

                swifty.setWindowVisibility(true)
            }

            function onHideWindow() {
                window.visible = false

                swifty.setWindowVisibility(false)
            }

            function onShowQml(fileUrl) {
                stack.push(customView, {"qmlUrl": fileUrl})
            }

            function onHomeScreen() {
                for (var i = stack.depth; i > 1; i--) {
                    stack.pop()
                }
            }

            function onShowPreviousPage() {
                if (stack.depth > 1) {
                    stack.pop();
                }
            }
        }

        Swifty {
            id: swifty
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
