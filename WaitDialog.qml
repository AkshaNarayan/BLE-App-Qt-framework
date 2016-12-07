import QtQuick 2.2
import QtQuick.Controls 1.2

import "."

Item {
    id: waitDialog
    width: 100
    height: 62
    visible: false
    property alias wait: wait
    property alias errPopUp: errPopUp
    property alias msgTimer: msgTimer

    MouseArea {
        id: ma
        anchors.fill: parent
        onClicked: {
            if(!waitDialog.visible)
                return
            errPopUp.opacity = 1
        }
    }

    Timer {
        id: msgTimer
        interval: 12000;
        onTriggered: {
            errPopUp.opacity = 0
            hide()
        }
    }

    Item {
        id: errPopUp
        property string text: ""
        width: ((parent.width*0.5) < Styles.mm(48)) ? (parent.width*0.5) : Styles.mm(48)
        height: Styles.mm(8)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: height
        Behavior on opacity { NumberAnimation { } }

        Rectangle {
            anchors.fill: parent
            color: "#000000"
            opacity: 0.7
            radius: Styles.mm(02)
        }

        BusyIndicator {
            id: wait
            width: 0;
            height: parent.height
            running: parent.visible
            style: Styles.busyStyle
            Component.onDestruction: {
                style = null
            }
        }

        Text {
            id: msg
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            width: parent.width - wait.width
            color: Styles.white
            font.pixelSize: Styles.dp16
            font.weight: Font.Medium
            horizontalAlignment: Text.AlignHCenter
            text: parent.text
            wrapMode: Text.WordWrap
        }
    }

    function show(msg, timeOut, bgEnabled) {
        visible = true
        if(msgTimer.running)
            msgTimer.stop()

        msgTimer.interval = timeOut*1000
        msgTimer.start()
        errPopUp.opacity = 1
        errPopUp.text = msg

        bgEnabled = typeof bgEnabled !== 'undefined' ? bgEnabled : true;
        ma.enabled = bgEnabled
    }

    function hide() {
        msgTimer.stop()
        errPopUp.opacity = 0
        errPopUp.text = ""
        visible = false
    }

    function quickHide(msg, timeOut) {
        visible = true
        msgTimer.stop()
        msgTimer.interval = timeOut*1000
        msgTimer.start()
        errPopUp.opacity = 1
        errPopUp.text = msg
    }
}

