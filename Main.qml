// application.qml
import QtQuick
import QtQuick.VectorImage

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
                    height: 2
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
                    height: 5
                }

                Item {
                    property bool selected: false

                    width: parent.width
                    height: contentRow.height
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 15
                    anchors.rightMargin: 15

                    // 背景层
                    Rectangle {
                        id: background
                        anchors.fill: parent
                        radius: 16
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop {
                                position: 0.0
                                color: "#ee0a24"
                            }
                            GradientStop {
                                position: 1.0
                                color: "#ff6b6b"
                            }
                        }
                        opacity: 0

                        Behavior on opacity {
                            NumberAnimation {
                                duration: 200
                                easing.type: Easing.OutCubic
                            }
                        }
                    }

                    // 内容层
                    Row {
                        id: contentRow
                        width: parent.width
                        topPadding: 12
                        bottomPadding: 12
                        leftPadding: 16
                        rightPadding: 16
                        spacing: 12

                        // 平移动画
                        transform: Translate {
                            id: translation
                            x: 0
                            Behavior on x {
                                NumberAnimation {
                                    duration: 200
                                    easing.type: Easing.OutCubic
                                }
                            }
                        }

                        VectorImage {
                            id: icon
                            width: 24
                            height: 24
                            anchors.verticalCenter: parent.verticalCenter
                            preferredRendererType: VectorImage.CurveRenderer
                            source: "qrc/icons/normal/play.svg"
                        }

                        Text {
                            id: text
                            text: '发现音乐'
                            font.pixelSize: 20
                            anchors.verticalCenter: parent.verticalCenter
                            color: '#999999'

                            Behavior on color {
                                ColorAnimation {
                                    duration: 200
                                    easing.type: Easing.OutCubic
                                }
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        onClicked: {
                            parent.selected = !parent.selected;
                            updateState();
                        }

                        onEntered: updateState()
                        onExited: if (!parent.selected)
                            updateState()

                        function updateState() {
                            let isActive = containsMouse || parent.selected;

                            text.color = isActive ? '#ee0a24' : '#999999';
                            icon.source = isActive ? "qrc/icons/active/play.svg" : "qrc/icons/normal/play.svg";
                            background.opacity = isActive ? 0.1 : 0;
                            translation.x = isActive ? 5 : 0;
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
