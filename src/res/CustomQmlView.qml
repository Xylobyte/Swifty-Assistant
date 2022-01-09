import QtQuick 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    anchors.fill: parent
    anchors.margins: 8
    spacing: 10

    property string qmlUrl

    Loader {
        id: loader
        Layout.fillHeight: true
        Layout.fillWidth: true
        source: qmlUrl
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
                qmlUrl = ""
                swifty.execAction("app home")
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
