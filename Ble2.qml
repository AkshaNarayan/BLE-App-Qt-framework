import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

Rectangle {
    id:mainRect
    Connections{
        target: ble
        onDisconnected2:{
            imageLight.imgObj=false;
            mainRect.visible=false;
            container.visible =true;
        }


    }

    color:"lightsteelblue"
    visible:false;
    width: 3000;
    height:1000;
    anchors.centerIn: parent;
    //    Text{
    //        text:"Your device is connected now. "
    //        color: "darkblue"
    //         }



    Image{
        id:imageLight
        source:"qrc:/1460460700_Light Bulb Off.png"
        property bool  imgObj: false;
        anchors.centerIn: parent;
    }

    Button{
        id:onButton
        text:"Back"
        height: 200;
        onClicked: {
            ble2qml.visible=false;
            if( container.visible ==false)
            {container.visible =true;}
        }
    }

    Button{
        id:offButton
        text:"On/Off"
        width:parent.width;
        height: 200;
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter;
        onClicked: {

            if (imageLight.imgObj==true)
            {
                imageLight.imgObj=false;
                imageLight. source="qrc:/1460460700_Light Bulb Off.png"
                ble.bulbOff();
            }
            else {
                imageLight.imgObj=true;
                imageLight.source="qrc:/1460460764_Light Bulb On.png"
                 ble.bulbOn();
            }
        }


    }
}

