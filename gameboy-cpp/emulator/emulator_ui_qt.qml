import QtCore
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs
import Emulator.UI

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
        parent: root.contentItem

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

        smooth: false
    }

    menuBar: MenuBar {
        Menu {
            title: "File"

            Action {
                text: "Load ROM"
                onTriggered: romFileDialog.open()
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
}