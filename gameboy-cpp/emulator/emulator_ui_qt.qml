import QtCore
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Gameboy.UI

ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: "Gameboy-cpp"

    background: FramebufferView {
        id: viewport
        backend: Backend
        width: framebufferWidth
        height: framebufferHeight
        smooth: false

        transform: [
            Scale {
                id: scale;
                yScale: Math.min(viewport.parent.width / viewport.width, viewport.parent.height / viewport.height);
                xScale: Math.min(viewport.parent.width / viewport.width, viewport.parent.height / viewport.height);
            },
            Translate {
                x: (viewport.parent.width - viewport.width  * scale.xScale) / 2;
                y: (viewport.parent.height - viewport.height * scale.yScale) / 2;
            }]
    }

    menuBar: MenuBar {
        Menu {
            title: "File"

            Action {
                text: "Load ROM"
                onTriggered: romFileDialog.open()
            }
        }

        Menu {
            title: "Debug"

            Action {
                text: "OAM Viewer"
                onTriggered: oamViewer.visible = true
            }
        }
    }

    FileDialog {
        id: romFileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        fileMode: FileDialog.OpenFile
        nameFilters: ["Rom Files (*.gb)"]
        onAccepted:{
            const result = Backend.load_rom(selectedFile)

            if(!result.success) {
                errorDialog.text = "Error loading rom file"
                errorDialog.informativeText = result.error
                errorDialog.open()
            }
        }
    }

    MessageDialog {
        id: errorDialog
        buttons: MessageDialog.Ok
    }

    Window {
        id: oamViewer
        visible: false
        title: "OAM Viewer"
        transientParent: root
        width: 1000
        height: 600

        RowLayout {
            anchors.fill: parent
            spacing: 10

            GridView {
                id: oamObjectGridView
                cellWidth: parent.width / 4
                Layout.preferredWidth: cellWidth * 2
                Layout.fillHeight: true

                ListModel {
                    id: oamObjectModel

                    ListElement { name: "Jim Williams" }
                    ListElement { name: "John Brown" }
                    ListElement { name: "Bill Smith" }
                    ListElement { name: "Sam Wise" }
                    ListElement { name: "Sam Wise" }
                    ListElement { name: "Sam Wise" }
                    ListElement { name: "Sam Wise" }
                    ListElement { name: "Sam Wise" }
                    ListElement { name: "Sam Wise" }
                    ListElement { name: "Sam Wise" }
                }

                Component {
                    id: oamObjectDelegate
                    Item {
                        width: oamObjectGridView.cellWidth
                        height: oamObjectGridView.cellHeight
                        Text { text: name }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: oamObjectGridView.currentIndex = index

                        }
                    }
                }

                model: oamObjectModel
                delegate: oamObjectDelegate
                highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                focus: true
                Component.onCompleted: currentIndex = -1
            }

            FramebufferView {
                id: oamDebugViewport
                Layout.fillHeight: true
                Layout.fillWidth: true
                backend: Backend
                smooth: false
            }
        }
    }
}