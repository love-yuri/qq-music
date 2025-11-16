// application.qml
import QtQuick
import QtQuick.VectorImage
import './components' as Compoents

Window {
    width: 520
    height: 1080
    visible: true
    color: '#FF99CC'

    Item {
        anchors.fill: parent
        Rectangle {
            id: leftPanel
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 300
            color: Qt.rgba(975, 985, 945, 935)

            Column {
                width: parent.width
                anchors.top: parent.top
                anchors.topMargin: 5
                spacing: 5
                property int selectIndex: 0
                Row {
                    padding: 5
                    anchors.horizontalCenter: parent.horizontalCenter

                    VectorImage {
                        width: 32
                        height: 32
                        anchors.verticalCenter: parent.verticalCenter
                        preferredRendererType: VectorImage.CurveRenderer
                        source: "qrc/icons/music-163.svg"
                    }

                    Text {
                        text: '网易云音乐'
                        font.pixelSize: 26
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Item {
                    width: parent.width
                    height: 5
                }

                Rectangle {
                    height: 1
                    width: parent.width
                    color: '#555555'
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 30
                    anchors.leftMargin: 30
                    opacity: 0.3
                }

                Item {
                    width: parent.width
                    height: 8
                }

                Compoents.MenuItem {
                    base_icon: 'play.svg'
                    title: '发现音乐'
                    selected: parent.selectIndex == 0
                    onItemClicked: {
                        parent.selectIndex = 0
                    }
                }

                Compoents.MenuItem {
                    base_icon: 'play.svg'
                    title: '私人FM'
                    selected: parent.selectIndex == 1
                    onItemClicked: {
                        parent.selectIndex = 1
                    }
                }
            }
        }

        Rectangle {
            id: divider
            anchors.left: leftPanel.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 2
            color: '#837369'
            opacity: 0.71

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.SizeHorCursor

                onMouseXChanged: {
                    if (pressed) {
                        var newWidth = leftPanel.width + mouseX;
                        // 限制宽度范围
                        if (newWidth >= 300 && newWidth <= 400) {
                            leftPanel.width = newWidth;
                        }
                    }
                }
            }
        }

        Rectangle {
            anchors.left: divider.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: '#FFCCFF'
        }
    }
}
