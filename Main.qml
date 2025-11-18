pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.VectorImage
import './components' as Compoents

Window {
    id: window
    width: 520
    height: 1080
    visible: true
    color: '#FFF5F8'

    property string currentPage: "views/DiscoverPage.qml"

    Item {
        id: root
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
                    color: '#FF6B9D'
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
            width: 300
            color: '#FFF9FB'

            Column {
                property int selectIndex: 0
                id: list
                width: parent.width
                anchors.top: parent.top
                anchors.topMargin: 20
                spacing: 2

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
                            page: 'views/DiscoverPage.qml'
                        }
                        ListElement {
                            icon: 'fm.svg'
                            text: '私人FM'
                            page: 'views/FMPage.qml'
                        }
                        ListElement {
                            icon: 'video.svg'
                            text: '视频'
                            page: 'views/VideoPage.qml'
                        }
                        ListElement {
                            icon: 'friend.svg'
                            text: '朋友'
                            page: 'views/FriendPage.qml'
                        }
                    }

                    delegate: Compoents.MenuItem {
                        required property int index
                        required property string text
                        required property string icon
                        required property string page

                        base_icon: icon
                        title: text
                        selected: index == list.selectIndex
                        onItemClicked: {
                            list.selectIndex = index
                            root.switchPage(page)
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
                            page: 'views/LocalMusicPage.qml'
                        }
                        ListElement {
                            icon: 'my-likes.svg'
                            text: '我喜欢的音乐'
                            page: 'views/MyLikesPage.qml'
                        }
                        ListElement {
                            icon: 'podcast.svg'
                            text: '播客'
                            page: 'views/PodcastPage.qml'
                        }
                    }

                    delegate: Compoents.MenuItem {
                        required property int index
                        required property string text
                        required property string icon
                        required property string page

                        base_icon: icon
                        title: text
                        selected: (index + 4) == list.selectIndex
                        onItemClicked: {
                            list.selectIndex = index + 4
                            root.switchPage(page)
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
                            if (newWidth >= 250 && newWidth <= 450) {
                                leftPanel.width = newWidth
                            }
                        }
                    }
                }
            }
        }

        // 主内容区域 - 简单直接的页面切换
        Rectangle {
            id: contentContainer
            anchors.left: divider.right
            anchors.right: parent.right
            anchors.top: titleBar.bottom
            anchors.bottom: parent.bottom
            color: '#FFF5F8'

            Loader {
                id: pageLoader
                anchors.fill: parent
                source: window.currentPage
            }
        }

        // 切换页面函数
        function switchPage(pageUrl) {
            if (pageUrl === window.currentPage) return
            window.currentPage = pageUrl
            pageLoader.source = pageUrl
        }
    }
}