import QtCore
import QtQuick
import QtQuick.Window
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import Gameboy.UI

ApplicationWindow {
    required property EmulatorControls controls
    required property EmulatorFramebuffer framebuffer

    required property bool debugMode
    property EmulatorSprites sprites
    property EmulatorBackground bg

    id: root
    width: 640
    height: 480
    visible: true
    title: "Gameboy-cpp"

    Material.theme: Material.Dark

    EmulatorVideo {
        visible: root.controls.status !== EmulatorStatus.Stopped
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
            enabled: root.controls.status !== EmulatorStatus.Stopped
            title: "Emulation"

            Action {
                enabled: root.controls.status === EmulatorStatus.Paused
                text: "Resume"
                onTriggered: root.controls.resume()
            }

            Action {
                enabled: root.controls.status === EmulatorStatus.Running
                text: "Pause"
                onTriggered: root.controls.pause()
            }

            Action {
                text: "Stop"
                onTriggered: root.controls.stop()
            }
        }

        Menu {
            id: debugMenu
            title: "Debug"

            Action {
                text: "Sprites Viewer"
                onTriggered: spritesViewerWindow.active = true
                enabled: root.controls.status !== EmulatorStatus.Stopped
            }

            Action {
                text: "Background Viewer"
                onTriggered: backgroundViewerWindow.active = true
                enabled: root.controls.status !== EmulatorStatus.Stopped
            }
        }

        Component.onCompleted: if(!root.debugMode) removeMenu(debugMenu)
    }

    ToolButton {
        id: volumeSliderButton
        enabled: root.controls.status !== EmulatorStatus.Stopped
        parent: root.menuBar
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: parent.height
        icon.name: {
            if(root.controls.muted)
                return "audio-volume-muted";

            const volume = root.controls.volume;

            if(volume > 0.65)
                return "audio-volume-high"

            if(volume > 0.33)
                return "audio-volume-medium";

            return "audio-volume-low";
        }

        display: AbstractButton.IconOnly
        onClicked: root.controls.muted = !root.controls.muted

        onHoveredChanged: {
            if (hovered && enabled)
                volumeSliderPopup.open()
        }

        Popup {
            id: volumeSliderPopup
            popupType: Popup.Item
            width: volumeSliderButton.width
            closePolicy: Popup.NoAutoClose;
            margins: 10
            y: root.menuBar.height + margins

            Slider {
                id: volumeSlider
                from: 0
                to: 1
                orientation: Qt.Vertical
                anchors.fill: parent
                value: root.controls.volume
                onMoved: root.controls.volume = position
            }

            HoverHandler {
                id: volumeSliderPopupBackgroundHoverArea
                parent: volumeSliderPopup.background
                enabled: parent.visible
            }

            Timer {
                interval: 50
                running: volumeSliderPopup.visible
                    && !volumeSliderButton.hovered
                    && !volumeSlider.hovered
                    && !volumeSliderPopupBackgroundHoverArea.hovered
                    && !volumeSliderPopupGapMouseArea.containsMouse
                onTriggered: volumeSliderPopup.visible = volumeSliderButton.hovered
                    || volumeSlider.hovered
                    || volumeSliderPopupBackgroundHoverArea.hovered
                    || volumeSliderPopupGapMouseArea.containsMouse
            }
        }

        MouseArea {
            id: volumeSliderPopupGapMouseArea
            enabled: volumeSliderPopup.visible
            hoverEnabled: true
            anchors { top: volumeSliderButton.bottom; }
            width: volumeSliderPopup.width
            height: volumeSliderPopup.y - volumeSliderButton.y + volumeSliderButton.height
        }
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

                        model: root.sprites
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
                                    root.controls.nextFrame()
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
                                onClicked: root.sprites.refreshSpritesCache()
                                enabled: controls.status !== EmulatorStatus.Running
                            }
                        }
                    }
                }
            }
        }
    }

    Loader {
        id: backgroundViewerWindow;
        active: false;
        sourceComponent: ApplicationWindow {
            id: backgroundViewer
            visible: true
            title: "Background Viewer"
            transientParent: root
            width: 1000
            height: 600
            onClosing: backgroundViewerWindow.active = false

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10

                Item {
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width / 2

                    EmulatorVideo {
                        id: emulatorBackgroundVideo
                        anchors.centerIn: parent
                        source: root.bg
                        width: Math.min(parent.width, parent.height * implicitWidth / implicitHeight)
                        height: width * implicitHeight / implicitWidth

                        Grid {
                            id: backgroundInspectorGrid
                            anchors.fill: parent
                            columns: 32
                            rows: 32

                            property int currentIndex: -1

                            Repeater {
                                model: backgroundInspectorGrid.columns * backgroundInspectorGrid.rows
                                Rectangle {
                                    readonly property int tileIndex: index

                                    width: backgroundInspectorGrid.width / backgroundInspectorGrid.columns
                                    height: backgroundInspectorGrid.height / backgroundInspectorGrid.rows
                                    color: "transparent"
                                    border.color: backgroundInspectorGrid.currentIndex === index ? "blue" : "lightsteelblue"
                                    border.width: 1

                                    MouseArea {
                                        anchors.fill: parent
                                        enabled: parent.enabled
                                        hoverEnabled: true
                                        preventStealing: true
                                        onEntered: backgroundInspectorGrid.currentIndex = parent.tileIndex
                                        onExited: backgroundInspectorGrid.currentIndex
                                            = backgroundInspectorGrid.currentIndex === parent.tileIndex
                                                ? -1
                                                : backgroundInspectorGrid.currentIndex
                                    }
                                }
                            }

                            Popup {
                                visible: parent.visible && backgroundInspectorGrid.currentIndex !== -1
                                enabled: false
                                popupType: Popup.Item
                                closePolicy: Popup.NoAutoClose
                                width: 120
                                height: 150
                                x: backgroundInspectorGrid.x
                                    + (backgroundInspectorGrid.width / backgroundInspectorGrid.columns)
                                    * (backgroundInspectorGrid.currentIndex % backgroundInspectorGrid.columns + 2)

                                y: backgroundInspectorGrid.y
                                    + (backgroundInspectorGrid.height / backgroundInspectorGrid.rows)
                                    * (backgroundInspectorGrid.currentIndex / backgroundInspectorGrid.rows)
                                    - height * 0.8

                                ColumnLayout {
                                    anchors.fill: parent

                                    Item {
                                        Layout.fillHeight: true
                                        Layout.fillWidth: true

                                        Image {
                                            anchors.fill: parent
                                            fillMode: Image.PreserveAspectFit
                                            cache: false
                                            smooth: false
                                            source: "image://background?tile_index=%1".arg(backgroundInspectorGrid.currentIndex)
                                        }
                                    }

                                    Text { text: "Index: %1".arg(backgroundInspectorGrid.currentIndex) }
                                    Text {
                                        text: "Position: (%1, %2)"
                                            .arg(backgroundInspectorGrid.currentIndex % backgroundInspectorGrid.columns)
                                            .arg(Math.trunc(backgroundInspectorGrid.currentIndex / backgroundInspectorGrid.columns))
                                    }
                                }
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width / 2

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10

                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            EmulatorVideo {
                                anchors.centerIn: parent
                                source: root.framebuffer
                                width: Math.min(parent.width, parent.height * implicitWidth / implicitHeight)
                                height: width * implicitHeight / implicitWidth
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
                                    root.controls.nextFrame()
                                }
                            }

                            RoundButton {
                                width: 100;
                                text: "Update background";
                                radius: 3;
                                icon.name: "view-refresh";
                                display: AbstractButton.IconOnly
                                ToolTip.text: text
                                ToolTip.visible: hovered
                                onClicked: root.bg.refresh()
                                enabled: controls.status !== EmulatorStatus.Running
                            }
                        }
                    }
                }
            }
        }
    }
}