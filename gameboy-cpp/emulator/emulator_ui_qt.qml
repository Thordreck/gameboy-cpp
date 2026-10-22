pragma ComponentBehavior: Bound

import QtCore
import QtQuick
import QtQuick.Window
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import Gameboy.UI

ApplicationWindow {
    id: root

    property EmulatorBackground bg
    required property EmulatorControls controls
    required property bool debugMode
    required property FileUtils fileUtils
    required property EmulatorFramebuffer framebuffer
    required property ProjectMetadata projectMetadata
    property EmulatorSprites sprites
    required property ThirdPartyMetadata thirdPartyMetadata

    Material.theme: Material.Dark
    height: 480
    title: "Gameboy-cpp"
    visible: true
    width: 640

    menuBar: MenuBar {
        Component.onCompleted: if (!root.debugMode)
            removeMenu(debugMenu)

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
                enabled: root.controls.status !== EmulatorStatus.Stopped
                text: "Sprites Viewer"

                onTriggered: spritesViewerWindow.active = true
            }
            Action {
                enabled: root.controls.status !== EmulatorStatus.Stopped
                text: "Background Viewer"

                onTriggered: backgroundViewerWindow.active = true
            }
        }
        Menu {
            title: "About"

            Action {
                text: "About"

                onTriggered: aboutProjectWindowLoader.active = true
            }
        }
    }

    EmulatorVideo {
        anchors.centerIn: parent
        height: width * implicitHeight / implicitWidth
        source: framebuffer
        visible: root.controls.status !== EmulatorStatus.Stopped
        width: Math.min(parent.width, parent.height * implicitWidth / implicitHeight)
    }
    ToolButton {
        id: volumeSliderButton

        anchors.right: parent.right
        anchors.rightMargin: 10
        display: AbstractButton.IconOnly
        enabled: root.controls.status !== EmulatorStatus.Stopped
        height: parent.height
        icon.name: {
            if (root.controls.muted)
                return "audio-volume-muted";

            const volume = root.controls.volume;

            if (volume > 0.65)
                return "audio-volume-high";

            if (volume > 0.33)
                return "audio-volume-medium";

            return "audio-volume-low";
        }
        parent: root.menuBar

        onClicked: root.controls.muted = !root.controls.muted
        onHoveredChanged: {
            if (hovered && enabled)
                volumeSliderPopup.open();
        }

        Popup {
            id: volumeSliderPopup

            closePolicy: Popup.NoAutoClose
            margins: 10
            popupType: Popup.Item
            width: volumeSliderButton.width
            y: root.menuBar.height + margins

            Slider {
                id: volumeSlider

                anchors.fill: parent
                orientation: Qt.Vertical
                to: 1
                value: root.controls.volume

                onMoved: root.controls.volume = position
            }
            HoverHandler {
                id: volumeSliderPopupBackgroundHoverArea

                enabled: parent.visible
                parent: volumeSliderPopup.background
            }
            Timer {
                interval: 50
                running: volumeSliderPopup.visible && !volumeSliderButton.hovered && !volumeSlider.hovered && !volumeSliderPopupBackgroundHoverArea.hovered && !volumeSliderPopupGapMouseArea.containsMouse

                onTriggered: volumeSliderPopup.visible = volumeSliderButton.hovered || volumeSlider.hovered || volumeSliderPopupBackgroundHoverArea.hovered || volumeSliderPopupGapMouseArea.containsMouse
            }
        }
        MouseArea {
            id: volumeSliderPopupGapMouseArea

            enabled: volumeSliderPopup.visible
            height: volumeSliderPopup.y - volumeSliderButton.y + volumeSliderButton.height
            hoverEnabled: true
            width: volumeSliderPopup.width

            anchors {
                top: volumeSliderButton.bottom
            }
        }
    }
    FileDialog {
        id: romFileDialog

        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        fileMode: FileDialog.OpenFile
        nameFilters: ["Rom Files (*.gb)"]

        onAccepted: {
            const result = root.controls.load_rom(selectedFile);

            if (!result.success) {
                errorDialog.text = "Error loading rom file";
                errorDialog.informativeText = result.error;
                errorDialog.open();
            }
        }
    }
    MessageDialog {
        id: errorDialog

        buttons: MessageDialog.Ok
    }
    Loader {
        id: spritesViewerWindow

        active: false

        sourceComponent: ApplicationWindow {
            id: spritesViewer

            height: 600
            title: "Sprites Viewer"
            transientParent: root
            visible: true
            width: 1000

            onClosing: spritesViewerWindow.active = false

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10

                Item {
                    Layout.fillHeight: true
                    Layout.preferredWidth: spritesViewer.width / 3

                    GridView {
                        id: spritesViewerGridView

                        anchors.fill: parent
                        cellHeight: spritesViewer.height / 4
                        cellWidth: spritesViewer.width / 6
                        delegateModelAccess: DelegateModel.ReadOnly
                        focus: true
                        keyNavigationEnabled: false
                        model: root.sprites

                        delegate: Item {
                            id: spriteDelegate

                            required property string imageURI
                            required property int index
                            required property bool priority
                            required property int spriteHeight
                            required property int spriteIndex
                            required property int spriteWidth
                            required property int spriteX
                            required property int spriteY
                            required property int tileIndex
                            required property bool xFlip
                            required property bool yFlip

                            height: GridView.view.cellHeight
                            width: GridView.view.cellWidth

                            Item {
                                anchors {
                                    centerIn: parent
                                    fill: parent
                                    margins: 8
                                }
                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 12

                                    Item {
                                        Layout.alignment: Qt.AlignCenter
                                        Layout.fillHeight: true
                                        Layout.fillWidth: true

                                        Image {
                                            anchors.fill: parent
                                            fillMode: Image.PreserveAspectFit
                                            smooth: false
                                            source: imageURI
                                        }
                                    }
                                    Item {
                                        Layout.fillHeight: true
                                        Layout.fillWidth: true

                                        Column {
                                            anchors.fill: parent

                                            Label {
                                                Layout.preferredWidth: width
                                                font.pointSize: 20
                                                fontSizeMode: Text.Fit
                                                height: parent.height / 6
                                                minimumPointSize: 2
                                                text: "Index: %1".arg(spriteDelegate.spriteIndex)
                                                width: parent.width
                                            }
                                            Label {
                                                Layout.preferredWidth: width
                                                font.pointSize: 20
                                                fontSizeMode: Text.Fit
                                                height: parent.height / 6
                                                minimumPointSize: 2
                                                text: "Pos: (%1, %2)".arg(spriteDelegate.spriteX).arg(spriteDelegate.spriteY)
                                                width: parent.width
                                            }
                                            Label {
                                                Layout.preferredWidth: width
                                                font.pointSize: 20
                                                fontSizeMode: Text.Fit
                                                height: parent.height / 6
                                                minimumPointSize: 2
                                                text: "Tile: %1".arg(spriteDelegate.tileIndex)
                                                width: parent.width
                                            }
                                            Label {
                                                Layout.preferredWidth: width
                                                font.pointSize: 20
                                                fontSizeMode: Text.Fit
                                                height: parent.height / 6
                                                minimumPointSize: 2
                                                text: "X flip: %1".arg(spriteDelegate.xFlip)
                                                width: parent.width
                                            }
                                            Label {
                                                Layout.preferredWidth: width
                                                font.pointSize: 20
                                                fontSizeMode: Text.Fit
                                                height: parent.height / 6
                                                minimumPointSize: 2
                                                text: "Y flip: %1".arg(spriteDelegate.yFlip)
                                                width: parent.width
                                            }
                                            Label {
                                                Layout.preferredWidth: width
                                                font.pointSize: 20
                                                fontSizeMode: Text.Fit
                                                height: parent.height / 6
                                                minimumPointSize: 2
                                                text: "Priority: %1".arg(spriteDelegate.priority)
                                                width: parent.width
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
                        highlight: Rectangle {
                            color: "lightsteelblue"
                            radius: 5
                        }

                        Component.onCompleted: currentIndex = -1
                        onCurrentIndexChanged: {
                            spriteScreenHighlight.visible = currentIndex !== -1;

                            if (currentItem != null) {
                                spriteScreenHighlight.spriteWidth = currentItem.spriteWidth;
                                spriteScreenHighlight.spriteHeight = currentItem.spriteHeight;
                                spriteScreenHighlight.spriteX = currentItem.spriteX;
                                spriteScreenHighlight.spriteY = currentItem.spriteY;
                            }
                        }
                    }
                }
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10

                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            EmulatorVideo {
                                id: spriteEmulatorVideo

                                anchors.centerIn: parent
                                height: width * implicitHeight / implicitWidth
                                source: framebuffer
                                width: Math.min(parent.width, parent.height * implicitWidth / implicitHeight)

                                Rectangle {
                                    id: spriteScreenHighlight

                                    property int spriteHeight
                                    property int spriteWidth
                                    property int spriteX
                                    property int spriteY

                                    border.color: "red"
                                    border.width: 5
                                    color: "transparent"
                                    height: (spriteHeight * spriteEmulatorVideo.height) / spriteEmulatorVideo.implicitHeight
                                    visible: false
                                    width: (spriteWidth * spriteEmulatorVideo.width) / spriteEmulatorVideo.implicitWidth
                                    x: (spriteX - 8) * spriteEmulatorVideo.width / spriteEmulatorVideo.implicitWidth
                                    y: (spriteY - 16) * spriteEmulatorVideo.height / spriteEmulatorVideo.implicitHeight
                                }
                            }
                        }
                        RowLayout {
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: true

                            RoundButton {
                                ToolTip.text: text
                                ToolTip.visible: hovered
                                display: AbstractButton.IconOnly
                                icon.name: root.controls.status === EmulatorStatus.Running ? "media-playback-pause" : "media-playback-start"
                                radius: 3
                                text: root.controls.status === EmulatorStatus.Running ? "Pause" : "Play"
                                width: 100

                                onClicked: {
                                    if (root.controls.status === EmulatorStatus.Running)
                                        root.controls.pause();
                                    else
                                        root.controls.resume();
                                }
                            }
                            RoundButton {
                                ToolTip.text: text
                                ToolTip.visible: hovered
                                display: AbstractButton.IconOnly
                                icon.name: "media-seek-forward"
                                radius: 3
                                text: "Jump to next frame"
                                width: 100

                                onClicked: {
                                    root.controls.pause();
                                    root.controls.nextFrame();
                                }
                            }
                            RoundButton {
                                ToolTip.text: text
                                ToolTip.visible: hovered
                                display: AbstractButton.IconOnly
                                enabled: controls.status !== EmulatorStatus.Running
                                icon.name: "view-refresh"
                                radius: 3
                                text: "Update sprites"
                                width: 100

                                onClicked: root.sprites.refreshSpritesCache()
                            }
                        }
                    }
                }
            }
        }
    }
    Loader {
        id: backgroundViewerWindow

        active: false

        sourceComponent: ApplicationWindow {
            id: backgroundViewer

            height: 600
            title: "Background Viewer"
            transientParent: root
            visible: true
            width: 1000

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
                        height: width * implicitHeight / implicitWidth
                        source: root.bg
                        width: Math.min(parent.width, parent.height * implicitWidth / implicitHeight)

                        Grid {
                            id: backgroundInspectorGrid

                            property int currentIndex: -1

                            anchors.fill: parent
                            columns: 32
                            rows: 32

                            Repeater {
                                model: backgroundInspectorGrid.columns * backgroundInspectorGrid.rows

                                Rectangle {
                                    readonly property int tileIndex: index

                                    border.color: backgroundInspectorGrid.currentIndex === index ? "blue" : "lightsteelblue"
                                    border.width: 1
                                    color: "transparent"
                                    height: backgroundInspectorGrid.height / backgroundInspectorGrid.rows
                                    width: backgroundInspectorGrid.width / backgroundInspectorGrid.columns

                                    MouseArea {
                                        anchors.fill: parent
                                        enabled: parent.enabled
                                        hoverEnabled: true
                                        preventStealing: true

                                        onEntered: backgroundInspectorGrid.currentIndex = parent.tileIndex
                                        onExited: backgroundInspectorGrid.currentIndex = backgroundInspectorGrid.currentIndex === parent.tileIndex ? -1 : backgroundInspectorGrid.currentIndex
                                    }
                                }
                            }
                            Popup {
                                closePolicy: Popup.NoAutoClose
                                enabled: false
                                height: 150
                                popupType: Popup.Item
                                visible: parent.visible && backgroundInspectorGrid.currentIndex !== -1
                                width: 120
                                x: backgroundInspectorGrid.x + (backgroundInspectorGrid.width / backgroundInspectorGrid.columns) * (backgroundInspectorGrid.currentIndex % backgroundInspectorGrid.columns + 2)
                                y: backgroundInspectorGrid.y + (backgroundInspectorGrid.height / backgroundInspectorGrid.rows) * (backgroundInspectorGrid.currentIndex / backgroundInspectorGrid.rows) - height * 0.8

                                ColumnLayout {
                                    anchors.fill: parent

                                    Item {
                                        Layout.fillHeight: true
                                        Layout.fillWidth: true

                                        Image {
                                            anchors.fill: parent
                                            cache: false
                                            fillMode: Image.PreserveAspectFit
                                            smooth: false
                                            source: "image://background?tile_index=%1".arg(backgroundInspectorGrid.currentIndex)
                                        }
                                    }
                                    Label {
                                        text: "Index: %1".arg(backgroundInspectorGrid.currentIndex)
                                    }
                                    Label {
                                        text: "Position: (%1, %2)".arg(backgroundInspectorGrid.currentIndex % backgroundInspectorGrid.columns).arg(Math.trunc(backgroundInspectorGrid.currentIndex / backgroundInspectorGrid.columns))
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
                            Layout.fillHeight: true
                            Layout.fillWidth: true

                            EmulatorVideo {
                                anchors.centerIn: parent
                                height: width * implicitHeight / implicitWidth
                                source: root.framebuffer
                                width: Math.min(parent.width, parent.height * implicitWidth / implicitHeight)
                            }
                        }
                        RowLayout {
                            Layout.alignment: Qt.AlignCenter
                            Layout.fillWidth: true

                            RoundButton {
                                ToolTip.text: text
                                ToolTip.visible: hovered
                                display: AbstractButton.IconOnly
                                icon.name: root.controls.status === EmulatorStatus.Running ? "media-playback-pause" : "media-playback-start"
                                radius: 3
                                text: root.controls.status === EmulatorStatus.Running ? "Pause" : "Play"
                                width: 100

                                onClicked: {
                                    if (root.controls.status === EmulatorStatus.Running)
                                        root.controls.pause();
                                    else
                                        root.controls.resume();
                                }
                            }
                            RoundButton {
                                ToolTip.text: text
                                ToolTip.visible: hovered
                                display: AbstractButton.IconOnly
                                icon.name: "media-seek-forward"
                                radius: 3
                                text: "Jump to next frame"
                                width: 100

                                onClicked: {
                                    root.controls.pause();
                                    root.controls.nextFrame();
                                }
                            }
                            RoundButton {
                                ToolTip.text: text
                                ToolTip.visible: hovered
                                display: AbstractButton.IconOnly
                                enabled: controls.status !== EmulatorStatus.Running
                                icon.name: "view-refresh"
                                radius: 3
                                text: "Update background"
                                width: 100

                                onClicked: root.bg.refresh()
                            }
                        }
                    }
                }
            }
        }
    }
    Loader {
        id: textFileViewerLoader

        property string contents: ""
        property string title: ""

        function show(title: string, contents: string) {
            this.contents = contents;
            this.title = title;
            this.active = true;
        }

        active: false

        sourceComponent: ApplicationWindow {
            height: 300
            title: textFileViewerLoader.title
            transientParent: root
            visible: true
            width: 600

            onClosing: {
                parent.text = "";
                parent.title = "";
                textFileViewerLoader.active = false;
            }

            ScrollView {
                anchors {
                    fill: parent
                    margins: 8
                }
                TextArea {
                    horizontalAlignment: TextEdit.AlignJustify
                    readOnly: true
                    text: textFileViewerLoader.contents
                }
            }
        }
    }
    Loader {
        id: aboutProjectWindowLoader

        active: false

        sourceComponent: ApplicationWindow {
            id: aboutProjectWindow

            height: 300
            title: "About"
            transientParent: root
            visible: true
            width: 600

            onClosing: aboutProjectWindowLoader.active = false

            ColumnLayout {
                spacing: 7

                anchors {
                    fill: parent
                    margins: 8
                }
                Label {
                    Layout.alignment: Qt.AlignCenter
                    font.pointSize: 20
                    text: "%1".arg(root.projectMetadata.name)
                }
                Label {
                    Layout.alignment: Qt.AlignCenter
                    font.pointSize: 10
                    text: "%1".arg(root.projectMetadata.description)
                }
                Label {
                    Layout.alignment: Qt.AlignCenter
                    font.pointSize: 10
                    text: "Developed by %1".arg(root.projectMetadata.author)
                }
                RowLayout {
                    Layout.alignment: Qt.AlignCenter

                    Label {
                        font.pointSize: 10
                        text: "v%1 (%2)".arg(root.projectMetadata.version).arg(root.projectMetadata.hash)
                    }
                    Label {
                        text: '<html><style type="text/css"></style><a href="%1">Homepage</a></html>'.arg(root.projectMetadata.url)

                        onLinkActivated: link => Qt.openUrlExternally(link)

                        MouseArea {
                            acceptedButtons: Qt.NoButton
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                        }
                    }
                }
                Label {
                    Layout.alignment: Qt.AlignCenter
                    text: "Licensed under %1".arg(root.projectMetadata.license)
                }
                Item {
                    height: 20
                }
                ListView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    delegateModelAccess: DelegateModel.ReadOnly
                    model: root.thirdPartyMetadata
                    spacing: 5

                    delegate: Item {
                        id: thirdPartyMetadataItem

                        required property string license
                        required property string name
                        required property string url

                        height: 25
                        width: ListView.view.width

                        Item {
                            anchors {
                                fill: parent
                            }
                            RowLayout {
                                anchors {
                                    centerIn: parent
                                }
                                Label {
                                    text: '<html><style type="text/css"></style><a href="%1">%2</a></html>'.arg(thirdPartyMetadataItem.url).arg(thirdPartyMetadataItem.name)

                                    onLinkActivated: link => Qt.openUrlExternally(link)

                                    MouseArea {
                                        acceptedButtons: Qt.NoButton
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                    }
                                }
                                Label {
                                    text: '<html><style type="text/css"></style><a href="%1">(license)</a></html>'.arg(thirdPartyMetadataItem.license)

                                    onLinkActivated: link => root.fileUtils.read_file(link).visit(contents => textFileViewerLoader.show("%1 license".arg(thirdPartyMetadataItem.name), contents), error => print("Error reading file: %1".arg(error)))

                                    MouseArea {
                                        acceptedButtons: Qt.NoButton
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                    }
                                }
                            }
                        }
                    }
                    header: Label {
                        horizontalAlignment: Text.AlignHCenter
                        text: "Third Party Modules"
                        width: ListView.view.width
                    }
                }
            }
        }
    }
}