import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick 2.5

ApplicationWindow {
    title: qsTr("BLE App")
    width: 640
    height: 480
    visible: true

    property real dpi: Screen.pixelDensity
    function mm(number)
    {
        return Math.round(number * Math.floor(dpi))}

    Connections{
        target: ble

        onBleInfoMsg:{
            waitToast.show(msg, timeOut,bgEnabled);
        }

        onScanDevFound:{
            console.log("This is working ", name,mac)
            listModelHain.append({"name": name, "mac_address":mac})
        }

        onConnected:{
            console.log("The device is connected now.")
            ble2qml.visible=true;
            container.visible = !container.visible
        }

    }



    Item{
        id: container;
        height: parent.height
        width: parent.width

        Rectangle{
            height:parent.height;
            width:parent.width;
            color:"white"
            id:mainRect

            Rectangle{
                height: 200;
                width: parent.width;
                color: "lightpink";
                id:encasingTextRect;
                radius:28
                border.color: "lightcoral"
                border.width: 8
                Text{
                    id:selectDeviceText
                    text:"<b>Select a device</b></br></br>";
                    color: "black"
                    font.pointSize: 28
                    textFormat: Text.StyledText
                    anchors.centerIn: encasingTextRect;
                    //  bottomPadding: 30
                }}

            Button{
                id:onButton;
                text:"Scan";
                style:ButtonStyle {
                    background: Rectangle {
                        implicitWidth: 100
                        implicitHeight: 25
                        border.width: control.activeFocus ? 2 : 1
                        border.color: "#888"
                        radius: 4
                        gradient: Gradient {
                            GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                            GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                        }
                    }}

                width:parent.width;
                height: 180;
                anchors.horizontalCenter: parent.horizontalCenter;
                anchors.bottom: parent.bottom;
                onClicked: {
                    console.log("The button has been clicked")
                    ble.scan();


                }
            }


            ListModel{
                id:listModelHain

            }

            ListView{
                width:parent.width;
                height: (parent.height-onButton.height-180);
                //  anchors.bottom: selectDeviceText.bottom;
                anchors.top: encasingTextRect.bottom;
                model:listModelHain
                delegate: Rectangle{

                    height: 300
                    width: parent.width
                    color: "lightblue"
                    border.color: "lightsteelblue"
                    border.width: 6
                    radius:18
                        MouseArea{
                            anchors.fill: parent;
                            onClicked:  {  ble.connectToDevice(mac_address)
                                console.log("The rectangular area was clicked")
                            }}

                    Text{
                        width:ListView.view.width;
                        height: 100
                        color: "darkblue"
                        font.pointSize: 20
                        textFormat: Text.StyledText
                        text: "<br>Name:"+name+"<br>"+"Mac:"+mac_address+"<br>"
                    }}
            }
        }
    }

    Ble2 {
        height: parent.height;
        width: parent.width;
        id: ble2qml
    }

       WaitDialog{
           id:waitToast
           anchors.fill: container;
       }
}
