pragma Singleton

import QtQuick 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2

Item {
    id: styles

    property alias fontAquatico: fontAquatico
    property alias sliderStyle: sliderStyle
    property alias busyStyle: busyStyle
    property alias checkboxStyle: checkboxStyle
    property alias comboBoxStyle: comboBoxStyle
    property alias progressStyle: progressStyle
    property alias seekBarStyle: seekBarStyle
    property alias radioButtonStyle: radioButtonStyle
    property alias switchStyle: switchStyle
    property alias tabViewStyle: tabViewStyle
    property alias textfieldStyle: textfieldStyle

    property color bg: "#282828"
    property color black: "#000000"
    property color blue: "#006df0"
    property color grey: "#747474"
    property color orange: "#e25c14"
    property color white: "#ffffff"
    property color transparent: "#00000000"

    property real dpi: Screen.pixelDensity
    property real dp1: mm(0.159)
    property real dp8: mm(1.270)
    property real dp12: mm(1.905)
    property real dp14: mm(2.222)
    property real dp16: mm(2.540)
    property real dp18: mm(2.857)
    property real dp22: mm(3.492)
    property real dp24: mm(3.810)
    property real dp28: mm(4.445)
    property real dp36: mm(5.715)
    property real dp40: mm(6.350)
    property real dp48: mm(7.620)
    property real dp56: mm(8.890)
    property real dp72: mm(11.430)
    property real dp100: mm(15.875)
    property real dp160: mm(25.400)

    FontLoader {
        id: fontAquatico
        source: viewer.adjustPath("qml/XCam/res/xcam/font/aquatico_regular.OTF")
        onStatusChanged: console.log("FONT : ", status)
    }

    Component {
        id: sliderStyle
        SliderStyle {
            groove: Rectangle {
                implicitHeight: control.height*0.65
                implicitWidth: control.width
            }

            handle: Rectangle {
                anchors.centerIn: parent
                color: control.pressed ? "#f47b00" : "#ffffff"
                implicitHeight: 4*control.height
                implicitWidth: implicitHeight
                radius: implicitHeight/2
            }
        }
    }

    Component {
        id: busyStyle
        BusyIndicatorStyle {
            indicator: AnimatedImage {
                fillMode: Image.PreserveAspectFit
                playing: visible
                visible: control.running
                source: viewer.adjustPath("qml/XCam/res/xcam/styles/searching.png")
                RotationAnimator on rotation {
                    running: control.running
                    loops: Animation.Infinite
                    duration: 2000
                    from: 0 ; to: 360
                }
            }
        }
    }

    Component {
        id: checkboxStyle
        CheckBoxStyle {
            label: Component {
                Text {
                    height: control.height
                    color: white
                    text: control.text
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: dp14
                }
            }
            indicator: Rectangle {
                implicitWidth: 14
                implicitHeight: implicitWidth
                radius: 3
                color: black
                Image {
                    visible: control.checked
                    source: viewer.adjustPath("qml/XCam/res/xcam/playback/check_small.png")
                    anchors.margins: 1
                    anchors.fill: parent
                }
            }
        }
    }

    Component {
        id: comboBoxStyle
        ComboBoxStyle {
            textColor: white
            selectedTextColor: white
            selectionColor: grey
            background: Component {
                Rectangle {
                    width: control.width
                    height: control.height
                    color: black
                }
            }
            label: Component {
                Text {
                    height: control.height
                    color: white
                    text: control.text
                    verticalAlignment: Text.AlignBottom
                    font.pixelSize: dp16
                }
            }
        }
    }

    Component {
        id: progressStyle
        ProgressBarStyle {
            panel: Item {
                implicitHeight: control.height
                implicitWidth: control.width

                Rectangle {
                    anchors.fill: parent
                    color: "#f7bb7d"
                }

                Rectangle {
                    antialiasing: true
                    radius: 1
                    color: "#f47b00"
                    height: parent.height
                    width: parent.width * control.value / control.maximumValue
                }
            }
        }
    }

    Component {
        id: seekBarStyle
        ProgressBarStyle {
            panel: Item {
                height: control.height
                width: control.width
                Rectangle {
                    id: backpanel
                    anchors.fill: parent
                    border.color: white
                    border.width: 1
                    color: "#E6E6E6"
                    radius: 4
                    opacity: 0.5
                }
                Rectangle {
                    antialiasing: true
                    radius: backpanel.radius
                    color: white
                    height: parent.height
                    width: parent.width * control.value / control.maximumValue
                }
            }
        }
    }

    Component {
        id: radioButtonStyle
        RadioButtonStyle {
            indicator: Image {
                height: dp24
                width: height
                source: control.checked
                        ? viewer.adjustPath("qml/XCam/res/icons/styles/ic_checkbox_p.png")
                        : viewer.adjustPath("qml/XCam/res/icons/styles/ic_checkbox_n.png")
            }
            label: Row {
                height: control.height
                width: control.width
                spacing: height*0.25
                Image {
                    id: icon
                    anchors.verticalCenter: parent.verticalCenter
                    height: dp24
                    width: height
                    source: control.checked
                            ? viewer.adjustPath("qml/XCam/res/icons/styles/ic_photo_mode_p.png")
                            : viewer.adjustPath("qml/XCam/res/icons/styles/ic_photo_mode_n.png")
                }
                Text {
                    height: parent.height
                    width: parent.width- icon.width
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    color: control.checked ? white : grey
                    font.pixelSize: mm(2.222)
                    text: control.text
                }
            }
        }
    }

    Component {
        id: switchStyle
        SwitchStyle {
            groove: Rectangle {
                implicitWidth: control.width
                implicitHeight: control.height
                radius: 11
                border.color: control.checked ? orange : grey
                border.width: 1
                color: orange

                Text {
                    anchors.fill: parent
                    anchors.margins: 3
                    horizontalAlignment: control.checked ? Text.AlignLeft : Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    text: control.checked ? "Ble" : "Wifi"
                    font.pixelSize: mm(3.25)
                }
            }
            handle:Rectangle {
                implicitWidth: implicitHeight+10
                implicitHeight: control.height
                radius: implicitHeight/2
                border.color: control.checked ? orange : grey
                border.width: 1
                color: control.pressed ? grey : white
            }
        }
    }

    Component {
        id: tabViewStyle
        TabViewStyle {
            frameOverlap: 1
            tab: Rectangle {
                implicitWidth: (control) ? control.width*0.34 : 0
                implicitHeight: (control) ? control.height*0.35 : 0
                clip: true
                color: styleData.selected ? "#221815" :"#595757"

                Row {
                    anchors.centerIn: parent
                    height: parent.height
                    Image {
                        id: icon
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: height
                        source: styleData.selected ? ((control) ? control.getTab(styleData.index).playO : "") : ((control) ?control.getTab(styleData.index).playW : "")
                    }
                    Text {
                        id: text
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: height*0.45
                        text: styleData.title
                        color: styleData.selected ? "#E99915" : white
                    }
                }
            }
            frame: Rectangle { color: "#221815" }
            tabsAlignment: Qt.AlignHCenter
            tabOverlap: 4
        }
    }

    Component {
        id: textfieldStyle
        TextFieldStyle {
            background: Item {
                implicitHeight: 32
                implicitWidth: 320
                Rectangle {
                    anchors.top: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: orange
                    radius: height*0.5
                    height: mm(0.159)
                }
            }
        }
    }

    function mm(number) {
        return Math.round(number * Math.floor(dpi))
    }
}

