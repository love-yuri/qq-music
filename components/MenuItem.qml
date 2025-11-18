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
    implicitHeight: 44
    anchors.left: parent.left
    anchors.right: parent.right

    Rectangle {
        id: background
        anchors.fill: parent
        anchors.leftMargin: 15
        anchors.rightMargin: 15
        radius: 12
        color: root.selected ? "#FFE8F0" : (root.hovered ? "#FFF0F5" : "transparent")

        // 左侧彩色边框指示器（仅选中时显示）
        Rectangle {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: 3
            height: parent.height * 0.5
            radius: 2
            color: "#FF6B9D"
            opacity: root.selected ? 1 : 0

            Behavior on opacity {
                NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
            }
        }
    }

    Row {
        id: contentRow
        anchors.fill: parent
        anchors.leftMargin: 25
        anchors.rightMargin: 15
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
            width: 22
            height: 22
            anchors.verticalCenter: parent.verticalCenter
            preferredRendererType: VectorImage.CurveRenderer
            source: `../qrc/icons/${root.active ? "active" : "normal"}/${root.base_icon}`
        }

        Text {
            id: text
            text: root.title
            font.pixelSize: 15
            anchors.verticalCenter: parent.verticalCenter
            color: root.selected ? "#ee0a24" : (root.hovered ? "#FF8FB3" : "#999999")
            font.weight: root.selected ? Font.Medium : Font.Normal
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