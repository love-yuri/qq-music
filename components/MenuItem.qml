import QtQuick
import QtQuick.VectorImage

Item {
    id: root
    property bool selected: false
    property string base_icon: "play.svg"
    property string title: "发现音乐"

    // hover 状态
    property bool hovered: false

    // 当前是否处于激活（选中或 hover）
    property bool active: hovered || selected

    signal itemClicked

    width: parent.width
    implicitHeight: childrenRect.height
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 15

    // 背景渐变
    Rectangle {
        id: background
        anchors.fill: parent
        radius: 16
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: "#ee0a24" }
            GradientStop { position: 1.0; color: "#ff6b6b" }
        }
        opacity: root.active ? 0.1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
        }
    }

    Row {
        id: contentRow
        width: parent.width
        padding: 12
        spacing: 12

        transform: Translate {
            id: translation
            x: root.active ? 5 : 0

            Behavior on x {
                NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
            }
        }

        VectorImage {
            id: icon
            width: 24; height: 24
            preferredRendererType: VectorImage.CurveRenderer

            source: `../qrc/icons/${root.active ? "active" : "normal"}/${root.base_icon}`
        }

        Text {
            id: text
            text: root.title
            font.pixelSize: 20
            anchors.verticalCenter: parent.verticalCenter
            color: root.active ? "#ee0a24" : "#999999"

            Behavior on color {
                ColorAnimation { duration: 200; easing.type: Easing.OutCubic }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onClicked: root.itemClicked()

        onEntered: root.hovered = true
        onExited: root.hovered = false
    }
}
