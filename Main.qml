pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.VectorImage
import './components' as Compoents

Window {
    width: 520
    height: 1080
    visible: true
    color: '#FFF5F8'

    Item {
        anchors.fill: parent
        anchors.margins: 0

        // 顶部标题栏
        Rectangle {
            id: titleBar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 60
            color: '#FFFFFF'
            
            // 底部阴影效果
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: '#FFD6E5'
            }

            Row {
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                spacing: 12

                VectorImage {
                    width: 32
                    height: 32
                    anchors.verticalCenter: parent.verticalCenter
                    preferredRendererType: VectorImage.CurveRenderer
                    source: "qrc/icons/music-163.svg"
                }

                Text {
                    text: '网易云音乐'
                    font.pixelSize: 22
                    font.bold: true
                    color: '#b4262d'
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        // 左侧导航栏
        Rectangle {
            id: leftPanel
            anchors.left: parent.left
            anchors.top: titleBar.bottom
            anchors.bottom: parent.bottom
            width: 250
            color: '#FFF9FB'

            Column {
                property int selectIndex: 0
                id: list
                width: parent.width
                anchors.top: parent.top
                anchors.topMargin: 20
                spacing: 5

                // 导航标题
                Text {
                    text: '在线音乐'
                    font.pixelSize: 13
                    color: '#CC99AA'
                    leftPadding: 25
                    bottomPadding: 10
                    font.bold: true
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

                // 分隔线
                Rectangle {
                    height: 1
                    width: parent.width - 40
                    color: '#FFD6E5'
                    anchors.horizontalCenter: parent.horizontalCenter
                    opacity: 0.6
                    anchors.topMargin: 15
                    anchors.bottomMargin: 15
                }

                // 我的音乐标题
                Text {
                    text: '我的音乐'
                    font.pixelSize: 13
                    color: '#CC99AA'
                    leftPadding: 25
                    topPadding: 5
                    bottomPadding: 10
                    font.bold: true
                }

                Repeater {
                    model: ListModel {
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
                        selected: (index + 4) == list.selectIndex
                        onItemClicked: {
                            list.selectIndex = index + 4
                        }
                    }
                }
            }
        }

        // 可拖动分割线
        Rectangle {
            id: divider
            anchors.left: leftPanel.right
            anchors.top: titleBar.bottom
            anchors.bottom: parent.bottom
            width: 1
            color: '#FFD6E5'

            Rectangle {
                id: dividerHandle
                anchors.centerIn: parent
                width: 5
                height: parent.height
                color: 'transparent'

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.SizeHorCursor
                    hoverEnabled: true

                    onEntered: {
                        divider.color = '#FFAAC8'
                        divider.width = 2
                    }

                    onExited: {
                        divider.color = '#FFD6E5'
                        divider.width = 1
                    }

                    onMouseXChanged: {
                        if (pressed) {
                            var newWidth = leftPanel.width + mouseX
                            if (newWidth >= 250 && newWidth <= 350) {
                                leftPanel.width = newWidth
                            }
                        }
                    }
                }
            }
        }

        // 主内容区域
        Rectangle {
            anchors.left: divider.right
            anchors.right: parent.right
            anchors.top: titleBar.bottom
            anchors.bottom: parent.bottom
            color: '#FFF5F8'

            // 内容区域的内边距和装饰
            Rectangle {
                anchors.fill: parent
                anchors.margins: 30
                color: 'transparent'

                Text {
                    anchors.centerIn: parent
                    text: '✨ 内容区域 ✨'
                    font.pixelSize: 20
                    color: '#FFAAC8'
                    font.bold: true
                }
            }
        }
    }
}