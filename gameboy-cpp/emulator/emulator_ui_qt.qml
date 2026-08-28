import QtCore
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import Gameboy.UI

ApplicationWindow {
    required property EmulatorControls controls
    required property EmulatorFramebuffer framebuffer

    required property bool debugMode
    property EmulatorDebug debug

    id: root
    width: 640
    height: 480
    visible: true
    title: "Gameboy-cpp"

    EmulatorVideo {
        anchors.centerIn: parent
        source: framebuffer
        width: Math.min(parent.width, parent.height * implicitWidth / implicitHeight)
        height: width * implicitHeight / implicitWidth
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
            id: debugMenu
            title: "Debug"

            Action {
                text: "Sprites Viewer"
                onTriggered: spritesViewerWindow.active = true
                enabled: controls.status !== EmulatorStatus.Stopped
            }
        }

        Component.onCompleted: if(!root.debugMode) removeMenu(debugMenu)
    }

    FileDialog {
        id: romFileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        fileMode: FileDialog.OpenFile
        nameFilters: ["Rom Files (*.gb)"]
        onAccepted:{
            const result = root.controls.load_rom(selectedFile)

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

    Loader {
        id: spritesViewerWindow;
        active: false;
        sourceComponent: ApplicationWindow {
            id: spritesViewer
            visible: true
            title: "Sprites Viewer"
            transientParent: root
            width: 1000
            height: 600
            onClosing: spritesViewerWindow.active = false

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10

                Item
                {
                    Layout.fillHeight: true
                    Layout.preferredWidth: spritesViewer.width / 3

                    GridView {
                        anchors.fill: parent
                        id: spritesViewerGridView
                        cellWidth: spritesViewer.width / 6
                        cellHeight: spritesViewer.height / 4

                        model: root.debug.sprites
                        delegateModelAccess: DelegateModel.ReadOnly
                        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                        focus: true
                        keyNavigationEnabled: false

                        Component.onCompleted: currentIndex = -1

                        onCurrentIndexChanged: {
                            spriteScreenHighlight.visible = currentIndex !== -1

                            if(currentItem != null) {
                                spriteScreenHighlight.spriteWidth = currentItem.spriteWidth
                                spriteScreenHighlight.spriteHeight = currentItem.spriteHeight
                                spriteScreenHighlight.spriteX = currentItem.spriteX
                                spriteScreenHighlight.spriteY = currentItem.spriteY
                            }
                        }

                        delegate: Item {
                            required property int index
                            required property int spriteIndex
                            required property int spriteX
                            required property int spriteY
                            required property int tileIndex
                            required property bool priority
                            required property bool xFlip
                            required property bool yFlip
                            required property string imageURI
                            required property int spriteWidth
                            required property int spriteHeight

                            id: spriteDelegate
                            width: GridView.view.cellWidth
                            height: GridView.view.cellHeight

                            Item
                            {
                                anchors { centerIn: parent; fill: parent; margins: 8 }

                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 12

                                    Item {
                                        Layout.fillHeight: true
                                        Layout.fillWidth: true
                                        Layout.alignment: Qt.AlignCenter

                                        Image {
                                            anchors.fill: parent
                                            source: imageURI
                                            fillMode: Image.PreserveAspectFit
                                            smooth: false
                                        }
                                    }

                                    Item {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true

                                        Column {
                                            anchors.fill: parent

                                            Text {
                                                text: "Index: %1".arg(spriteDelegate.spriteIndex);
                                                fontSizeMode: Text.Fit;
                                                width: parent.width
                                                height: parent.height / 6
                                                minimumPointSize: 2;
                                                font.pointSize: 20

                                                Layout.preferredWidth: width
                                            }

                                            Text {
                                                text: "Pos: (%1, %2)".arg(spriteDelegate.spriteX).arg(spriteDelegate.spriteY);
                                                fontSizeMode: Text.Fit
                                                width: parent.width
                                                height: parent.height / 6
                                                minimumPointSize: 2;
                                                font.pointSize: 20

                                                Layout.preferredWidth: width
                                            }

                                            Text {
                                                text: "Tile: %1".arg(spriteDelegate.tileIndex);
                                                fontSizeMode: Text.Fit
                                                width: parent.width
                                                height: parent.height / 6
                                                minimumPointSize: 2;
                                                font.pointSize: 20

                                                Layout.preferredWidth: width
                                            }

                                            Text {
                                                text: "X flip: %1".arg(spriteDelegate.xFlip);
                                                fontSizeMode: Text.Fit
                                                width: parent.width
                                                height: parent.height / 6
                                                minimumPointSize: 2;
                                                font.pointSize: 20

                                                Layout.preferredWidth: width
                                            }

                                            Text {
                                                text: "Y flip: %1".arg(spriteDelegate.yFlip);
                                                fontSizeMode: Text.Fit
                                                width: parent.width
                                                height: parent.height / 6
                                                minimumPointSize: 2;
                                                font.pointSize: 20

                                                Layout.preferredWidth: width
                                            }

                                            Text {
                                                text: "Priority: %1".arg(spriteDelegate.priority);
                                                fontSizeMode: Text.Fit
                                                width: parent.width
                                                height: parent.height / 6
                                                minimumPointSize: 2;
                                                font.pointSize: 20

                                                Layout.preferredWidth: width
                                            }
                                        }
                                    }
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: spritesViewerGridView.currentIndex = index
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ColumnLayout
                    {
                        anchors.fill: parent
                        spacing: 10

                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            EmulatorVideo {
                                id: spriteEmulatorVideo
                                anchors.centerIn: parent
                                source: framebuffer

                                width: Math.min(parent.width, parent.height * implicitWidth / implicitHeight)
                                height: width * implicitHeight / implicitWidth

                                Rectangle {
                                    property int spriteWidth
                                    property int spriteHeight
                                    property int spriteX
                                    property int spriteY

                                    visible: false
                                    id: spriteScreenHighlight
                                    color: "transparent"
                                    border.color: "red"
                                    border.width: 5
                                    width: (spriteWidth * spriteEmulatorVideo.width) / spriteEmulatorVideo.implicitWidth;
                                    height: (spriteHeight * spriteEmulatorVideo.height) / spriteEmulatorVideo.implicitHeight;
                                    x: (spriteX - 8) * spriteEmulatorVideo.width / spriteEmulatorVideo.implicitWidth
                                    y: (spriteY - 16) * spriteEmulatorVideo.height / spriteEmulatorVideo.implicitHeight
                                }
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignCenter

                            RoundButton {
                                width: 100;
                                text: root.controls.status === EmulatorStatus.Running ? "Pause" : "Play";
                                radius: 3;
                                icon.name: root.controls.status === EmulatorStatus.Running ? "media-playback-pause" : "media-playback-start";
                                display: AbstractButton.IconOnly
                                ToolTip.text: text
                                ToolTip.visible: hovered
                                onClicked: {
                                    if (root.controls.status === EmulatorStatus.Running)
                                        root.controls.pause()
                                    else
                                        root.controls.resume()
                                }
                            }

                            RoundButton {
                                width: 100;
                                text: "Jump to next frame";
                                radius: 3;
                                icon.name: "media-seek-forward";
                                display: AbstractButton.IconOnly
                                ToolTip.text: text
                                ToolTip.visible: hovered
                                onClicked: {
                                    root.controls.pause()
                                    root.debug.nextFrame()
                                }
                            }

                            RoundButton {
                                width: 100;
                                text: "Update sprites";
                                radius: 3;
                                icon.name: "view-refresh";
                                display: AbstractButton.IconOnly
                                ToolTip.text: text
                                ToolTip.visible: hovered
                                onClicked: root.debug.sprites.refreshSpritesCache()
                                enabled: controls.status !== EmulatorStatus.Running
                            }
                        }
                    }
                }
            }
        }
    }
}