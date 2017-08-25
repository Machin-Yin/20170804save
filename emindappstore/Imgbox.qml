import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
//import Qt3D.Core 2.0
//import Qt3D.Render 2.0
//import QtQuick.Scene3D 2.0

Item {
    id:imgBox
    width:940
    height:220
    property int margin : 10

    RowLayout{
        z:1
        anchors.centerIn:   parent
        spacing: 800
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


    RowLayout{
        z:1
        id:layout1
        spacing:10
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 380

        Button{
            Layout.preferredWidth: 30
            Layout.preferredHeight:30
            onClicked: playLastImg();
        }
        Button{
            Layout.preferredWidth: 30
            Layout.preferredHeight:30
            onClicked: playNextImg();
        }
        Button{
            Layout.preferredWidth: 30
            Layout.preferredHeight:30
            onClicked: playLastImg();
        }
        Button{
            Layout.preferredWidth: 30
            Layout.preferredHeight:30
            onClicked: playNextImg();
        }
    }


    Image{
        id:img1
        x:0
        y:15
        z:-1
        width:300
        height: 180
        source:"qrc:/image/test1.jpg"
                transform:Rotation{
                    origin.x:0
                    origin.y:0
                    axis{
                        x:0
                        y:1
                        z:0
                    }
                    angle:30
                }
        smooth:true
    }


    Image{
        id:img2
        x:parent.width/2 - 260
        y:0
        z:0
        width:500
        height: 190
        source:"qrc:/image/test2.jpg"
        //        transform:Rotation{
        //            origin.x:0
        //            origin.y:0
        //            axis{
        //                x:0
        //                y:0
        //                z:0
        //            }
        //            angle:0
        //        }
        smooth:true
    }

    Image{
        id: img3
        x:600
        y:15
        z:-1
        width:300
        height: 160
        source:"qrc:/image/test3.jpg"
                transform:Rotation{
                    origin.x:0
                    origin.y:0
                    axis{
                        x: 0
                        y: -1
                        z: 0
                    }
                    angle:30
                }
        smooth:true

    }



    PathAnimation{
        id:animateImg
        target:img2
        duration: 5000
        easing.type:Easing.InCubic
        path:Path{
            startX: 0
            startY: 0
            pathElements: PathArc{
                x:360
                y:0

                useLargeArc:true
                radiusX: 160
                radiusY: 160
                direction: PathArc.Counterclockwise

            }

        }
    }

    //    ShaderEffect {
    //        anchors.top: img3.bottom
    //        width: img3.width
    //        height: img3.height
    //        anchors.left: img3.left

    //        property variant source: img3
    //        property size sourceSize: Qt.size(0.5 / img3.width, 0.5 / img3.height)

    //        fragmentShader: "
    //                varying highp vec2 qt_TexCoord0;
    //                uniform lowp sampler2D source;
    //                uniform lowp vec2 sourceSize;
    //                uniform lowp float qt_Opacity;
    //                void main() {

    //                    lowp vec2 tc = qt_TexCoord0 * vec2(1, -1) + vec2(0, 1);
    //                    lowp vec4 col = 0.25 * (texture2D(source, tc + sourceSize)
    //                                            + texture2D(source, tc- sourceSize)
    //                                            + texture2D(source, tc + sourceSize * vec2(1, -1))
    //                                            + texture2D(source, tc + sourceSize * vec2(-1, 1))
    //                                           );
    //                    gl_FragColor = col * qt_Opacity * (1.0 - qt_TexCoord0.y) * 0.2;
    //                }"
    //    }

    function playLastImg(){
        animateImg.start()
    }

    function playNextImg(){
        animateImg.start()
    }
}
