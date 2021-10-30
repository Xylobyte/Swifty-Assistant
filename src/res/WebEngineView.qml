import QtQuick 2.15
import QtWebEngine 1.10
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQml 2.2

Rectangle {
    anchors.fill: parent
    color: "#171717"
    radius: 10

    property string typeWeb: "web_with_action_btn"
    property string webUrl

    property QtObject defaultProfile: WebEngineProfile {
        storageName: "Profile"
        httpUserAgent: "Mozilla/5.0 (Linux; Android 9; SM-A102U) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.136 Mobile Safari/537.36"
        offTheRecord: false
        useForGlobalCertificateVerification: true
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8

        WebEngineView {
            id: webEngineView
            Layout.fillWidth: true
            Layout.fillHeight: true
            focus: true
            url: webUrl
            profile: defaultProfile

            settings.autoLoadImages: true
            settings.javascriptEnabled: true
            settings.errorPageEnabled: false
            settings.pluginsEnabled: false
            settings.fullScreenSupportEnabled: false
            settings.autoLoadIconsForPage: true
            settings.touchIconsEnabled: false
            settings.webRTCPublicInterfacesOnly: false
            settings.pdfViewerEnabled: true

            onCertificateError: function(error) {
                error.defer();
            }

            onNewViewRequested: function(request) {
                if (!request.userInitiated)
                    print("Warning: Blocked a popup window.");
                else {
                    url = request.openIn(webEngineView)
                }
            }

            onQuotaRequested: function(request) {
                if (request.requestedSize <= 5 * 1024 * 1024)
                    request.accept();
                else
                    request.reject();
            }

            onRegisterProtocolHandlerRequested: function(request) {
                console.log("accepting registerProtocolHandler request for "
                            + request.scheme + " from " + request.origin);
                request.accept();
            }

            onRenderProcessTerminated: function(terminationStatus, exitCode) {
                var status = "";
                switch (terminationStatus) {
                case WebEngineView.NormalTerminationStatus:
                    status = "(normal exit)";
                    break;
                case WebEngineView.AbnormalTerminationStatus:
                    status = "(abnormal exit)";
                    break;
                case WebEngineView.CrashedTerminationStatus:
                    status = "(crashed)";
                    break;
                case WebEngineView.KilledTerminationStatus:
                    status = "(killed)";
                    break;
                }

                print("Render process exited with code " + exitCode + " " + status);
                reloadTimer.running = true;
            }

            onSelectClientCertificate: function(selection) {
                selection.certificates[0].select();
            }

            onLoadingChanged: function(loadRequest) {

            }

            Timer {
                id: reloadTimer
                interval: 100
                running: false
                repeat: false
                onTriggered: webEngineView.reload()
            }
        }

        RowLayout {
            Layout.fillWidth: true

            MButton {
                text: "Accueil"
                font.pointSize: 8
                implicitHeight: 21
                borderWidth: 2
                radius: 10
                onClicked: {
                    webEngineView.stop()
                    swifty.execAction("app home")
                }
            }

            MButton {
                text: qsTr("Précédent")
                font.pointSize: 8
                implicitHeight: 21
                borderWidth: 2
                visible: typeWeb === "web_with_action_btn" ? true : false
                radius: 10
                onClicked: {
                    webEngineView.goBack()
                }
            }

            MButton {
                text: qsTr("Suivant")
                font.pointSize: 8
                implicitHeight: 21
                visible: typeWeb === "web_with_action_btn" ? true : false
                borderWidth: 2
                radius: 10
                onClicked: {
                    webEngineView.goForward()
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MButton {
                text: qsTr("Ouvrir le lien")
                font.pointSize: 8
                implicitHeight: 21
                borderWidth: 2
                visible: typeWeb === "web_with_action_btn" ? true : false
                radius: 10
                onClicked: {
                    swifty.openLinkInBrowser(webEngineView.url)
                }
            }
        }
    }
}
