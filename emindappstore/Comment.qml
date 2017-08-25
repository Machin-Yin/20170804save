import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
Item {
    id:commBox
    width: 900
    height: 640
    property int btnWidth : 30
    property int btnHeight : 30
    property int pgNumber : 1

    Rectangle{
        anchors.fill: parent
        //        width:640
        ColumnLayout{

            RowLayout{
                Text{
                    text:qsTr("user comment:")
                }
                Button{
                    text:qsTr("current version")

                }
                Button{
                    text:qsTr("history version")
                }
            }
            RowLayout{

                ListView{
                    Layout.preferredHeight: 400
                    Layout.preferredWidth:  850
                    delegate:delegate
                    model:commModel
                }

                ListModel{
                    id:commModel
                    ListElement{

                        name:"lily"
                        content:"nice"
                    }
                    ListElement{

                        name:"lijun"
                        content:"good"
                    }
                    ListElement{

                        name:"lilei"
                        content:"nice"
                    }
                    ListElement{

                        name:"lily"
                        content:"nice"
                    }
                    ListElement{
                        name:"lijun"
                        content:"good"
                    }
                    ListElement{

                        name:"lilei"
                        content:"nice"
                    }

                }

                Component{
                    id:delegate

                    RowLayout{
                        spacing:10
                        Image{
                            id:userHeadimg
                            source:"qrc:/image/Tux.png"
                        }

                        ColumnLayout{
                            spacing:10
                            RowLayout{
                                Text{
                                    id:userName
                                    text:name
                                }
                                RowLayout{
                                    Image{
                                        source:"qrc:/image/next.png"
                                    }
                                    Image{
                                        source:"qrc:/image/next.png"
                                    }
                                    Image{
                                        source:"qrc:/image/next.png"
                                    }
                                    Image{
                                        source:"qrc:/image/next.png"
                                    }
                                    Image{
                                        source:"qrc:/image/next.png"
                                    }
                                }
                            }
                            Text{
                                id:comment
                                text:content
                            }

                        }

                    }

                }
            }

            RowLayout{
                anchors.right: parent.right
                Button{
                    id:btnPrev
                    Layout.preferredWidth: btnWidth
                    Layout.preferredHeight:btnHeight
                    text: "<"
                    onClicked: commBox.pgNumber=commBox.pgNumber-1

                }
                Button{
                    id:btnPg1
                    Layout.preferredWidth: btnWidth
                    Layout.preferredHeight:btnHeight
                    text: pgNumber.toString()

                }
                Button{
                    id:btnPg2
                    Layout.preferredWidth: btnWidth
                    Layout.preferredHeight:btnHeight
                    text: (pgNumber+1).toString()

                }
                Button{
                    id:btnPg3
                    Layout.preferredWidth: btnWidth
                    Layout.preferredHeight:btnHeight
                    text: (pgNumber+2).toString()

                }
                Button{
                    id:btnPg4
                    Layout.preferredWidth: btnWidth
                    Layout.preferredHeight:btnHeight
                    text: (pgNumber+3).toString()

                }
                Button{
                    id:btnPg5
                    Layout.preferredWidth: btnWidth
                    Layout.preferredHeight:btnHeight
                    text: (pgNumber+4).toString()

                }
                Button{
                    id:btnNext
                    Layout.preferredWidth: btnWidth
                    Layout.preferredHeight:btnHeight
                    text: ">"
                    onClicked: {

                        commBox.pgNumber=commBox.pgNumer+1
                    }

                }
            }


        }
    }

    function minusPg(){

    }

    function plusPg(){

    }
}
