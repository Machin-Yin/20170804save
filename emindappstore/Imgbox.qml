import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import QtQuick.Scene3D 2.0

Item {
    id:imgBox
    width:940
    height:220
    property int margin : 10

    Row{
        id:layout
        width: 940
        height:120
        anchors.fill:parent
        anchors.topMargin: 70

        Button{
            width:50
            height:50
            anchors.left: parent.left
            iconSource: "qrc:/image/return.png"
            onClicked: playLastImg();
        }
        Button{
            width:50
            height:50
            anchors.right: parent.right
            iconSource:"qrc:/image/next.png"
            onClicked: playNextImg();
        }

    }

    Row{
        id:layout1
        width:300
        height:100
        anchors.fill:parent
        anchors.leftMargin: 420
        anchors.topMargin: 200
        Button{
            width:20
            height:20

            onClicked: playLastImg();
        }
        Button{
            width:20
            height:20
            onClicked: playNextImg();
        }
        Button{
            width:20
            height:20
            onClicked: playLastImg();
        }
        Button{
            width:20
            height:20
            onClicked: playNextImg();
        }
    }

//    Scene3D{
//        id:scene3d
//        anchors.fill:parent
//        focus:true
//        aspects: "input"
//        z:-2

//    }
    Row{
        z:-2
        Image{
            id:img1
            x:23
            y:50
            source:"qrc:/image/test.jpg"
            transform:Rotation{
                origin.x:50
                origin.y:50
                axis{
                    x:0
                    y:1
                    z:0
                }
                angle:70
            }
            smooth:true
        }
        Image{
            id:img2
            x:67
            y:20
            source:"qrc:/image/test.jpg"
            transform:Rotation{
                origin.x:30
                origin.y:90
                axis{
                    x:1
                    y:1
                    z:0
                }
                angle:50
            }
            smooth:true
        }

        Image{
            id:img3
            x:100
            y:20
            source:"qrc:/image/test.jpg"
            transform:Rotation{
                origin.x:30
                origin.y:30
                axis{
                    x:0
                    y:1
                    z:0
                }
                angle:30
            }
            smooth:true
        }
    }



    function playLastImg(){

    }

    function playNextImg(){

    }
}
