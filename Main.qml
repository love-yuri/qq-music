pragma ComponentBehavior: Bound

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
                property int selectIndex: 0
                id: list
                width: parent.width
                anchors.top: parent.top
                anchors.topMargin: 5
                spacing: 5
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

                Repeater {
                    model: ListModel {
                        ListElement {
                            icon: 'play.svg'
                            text: '发现音乐'
                        }
                        ListElement {
                            icon: 'fm.svg'
                            text: '私人FM'
                        }
                        ListElement {
                            icon: 'video.svg'
                            text: '视频'
                        }
                        ListElement {
                            icon: 'friend.svg'
                            text: '朋友'
                        }
                        ListElement {
                            icon: 'local-music.svg'
                            text: '本地音乐'
                        }
                        ListElement {
                            icon: 'podcast.svg'
                            text: '播客'
                        }
                    }

                    delegate: Compoents.MenuItem {
                        required property int index
                        required property string text
                        required property string icon

                        base_icon: icon
                        title: text
                        selected: index == list.selectIndex
                        onItemClicked: {
                            list.selectIndex = index
                        }
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
