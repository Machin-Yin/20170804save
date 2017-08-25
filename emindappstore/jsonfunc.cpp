#include "jsonfunc.h"
#include "qreplytimeout.h"
#include <QDebug>

JSONFUNC::JSONFUNC(ShareData *shareData)
{
    manager = new QNetworkAccessManager();
    jsonData = shareData;
    jsonFlag = 0;
    categoryNum = 0;
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(jsonAnalysis(QNetworkReply*)),Qt::QueuedConnection);
    ipString = "http://127.0.0.1:8888/";
    iconString = "http://127.0.0.1:8888/icon/";
}

//获取分类数目
void JSONFUNC::getCategoryNum()
{
    jsonFlag = CATEGORIES;
    QString httpAddress = ipString + "categories";
    manager->get(QNetworkRequest(QUrl(httpAddress)));
}

//array -> releaseId , size -> product Number
//get jsondata to sharedata
void JSONFUNC::getUpdateRelease(int *array,int size)
{
//    qDebug() << __FUNCTION__;
    QByteArray dataArr;
    QString dataStr = "[";
    for(int i=0;i<size;i++)
    {
        if(i == 0)
            dataStr += QString::number(array[i]);
        else
        {
            dataStr += (",");
            dataStr += QString::number(array[i]);
        }
    }
    dataStr += "]";

    dataArr = dataStr.toLatin1();
//        qDebug()<<"dataArr == "<<dataArr<<endl;

    QString baseUrl = ipString +"releases";
    QUrl url(baseUrl);
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(url);

    jsonFlag = RELEASE;
    manager->post(request,dataArr);
}

//设置软件名字
void JSONFUNC::setAppname()
{
    jsonFlag = PRODUCTS;
    QString httpAddress = ipString + "products";
    manager->get(QNetworkRequest(QUrl(httpAddress)));
//    manager->get(QNetworkRequest(QUrl("http://127.0.0.1:8888/products")));
}

void JSONFUNC::getRecommend()
{
    jsonFlag = RECOMMEND;
    QString httpAddress = ipString + "recommends";
    manager->get(QNetworkRequest(QUrl(httpAddress)));
}

void JSONFUNC::getScreenImage()
{
    jsonFlag = SCRIMAGE;
    //    manager->get();
}


void JSONFUNC::getComment(int *array)
{
    jsonFlag = COMMENTDATA;
    QByteArray dataArr;
    QString dataStr = "[";
    for(int i=0;i<5;i++)
    {
        if(i == 0)
            dataStr += QString::number(array[i]);
        else
        {
            dataStr += (",");
            dataStr += QString::number(array[i]);
        }
    }
    dataStr += "]";

    dataArr = dataStr.toLatin1();
    //    qDebug()<<"dataArr == "<<dataArr<<endl;

    QString baseUrl = ipString + "comments";
    QUrl url(baseUrl);
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(url);
    manager->post(request,dataArr);
}

void JSONFUNC::jsonAnalysis(QNetworkReply *reply)
{
    QByteArray dataRead;
    dataRead = reply->readAll();

    QReplyTimeout *pTimeout = new QReplyTimeout(reply, 1000);
    // 超时进一步处理
    connect(pTimeout, &QReplyTimeout::timeout, [=]() {
        qDebug() << "Timeout";
    });

    QJsonParseError jsonerror;
    QJsonDocument document = QJsonDocument::fromJson(dataRead,&jsonerror);

    if(jsonerror.error == QJsonParseError::NoError)
    {
        if(document.isObject())
        {
            QJsonObject obj = document.object();
            if(jsonFlag == CATEGORIES)
            {
                getCategoryNum(obj);
            }
            else if(jsonFlag == PRODUCTS)
            {
                getProducts(obj);
            }
            else if(jsonFlag == RELEASE)
            {
                getUpdateRelease(obj);
            }
            else if(jsonFlag == RECOMMEND)
            {
                getRecommend(obj);
            }
            else if(jsonFlag == SCRIMAGE)
            {
                getScreenImage(obj);
            }
            else if (jsonFlag == COMMENTDATA)
            {
                getComment(obj);
            }
        }
    }
    else {
        qDebug()<<"json is error"<<endl;
    }
    reply->deleteLater();
}

void JSONFUNC::getCategoryNum(QJsonObject obj)
{
    if(obj.contains("Categories"))
    {
        QJsonValue test = obj.take("Categories");
        if(test.isArray())
        {
            QJsonArray str = test.toArray();
            int size = str.size();
            categoryNum = size;

            for(int i = 0;i < size;i++ )
            {
                QJsonValue value = str.at(i);

                if(value.isObject())
                {
                    int cate = 0;
                    QString name;
                    QJsonObject obj2 = value.toObject();

                    if(obj2.contains("ID"))
                    {
                        QJsonValue category = obj2.take("ID");
                        if(category.isDouble())
                        {
                            cate = category.toInt();
                        }
                    }
                    else
                    {
                        cate = 0;
                    }

                    if(obj2.contains("category_name"))
                    {
                        QJsonValue categoryname = obj2.take("category_name");
                        if(categoryname.isString())
                        {
                            name = categoryname.toString();
                            //                                        qDebug()<<name<<endl;
                        }
                    }
                    else
                    {
                        name = "";
                    }

                    if(cate != 0 && name != "")
                        jsonData->categoryMap[cate] = name;
                }
            }
        }
        //                    qDebug()<<"categoryNum  ==  "<<categoryNum<<endl;
        emit categoryIsOk(categoryNum);
    }
}

void JSONFUNC::getProducts(QJsonObject obj)
{
    if(obj.contains("products"))
    {
        QJsonValue pro = obj.take("products");
        if(pro.isArray())
        {
            QJsonArray str = pro.toArray();
            int size = str.size();
            QJsonValue categoryid ;

            int lnProductId = 0;
            int relid = 0;
            int cateid = 0;
            int relcate = 0;
            double gracount = 0;
            QString proname;
            QString venname;
            QString icourl;
            QString prourl;
            QString prodesc;
            QString exefile;
            QString packname;

            QMap<int,int>::iterator it;
            int y = 0;

            for(int i = 0;i < size;i++)
            {
                QJsonValue value = str.at(i);

                if(value.isObject())
                {
                    QJsonObject obj2 = value.toObject();

                    if(obj2.contains("ID"))
                    {
                        QJsonValue productid = obj2.take("ID");
                        if(productid.isDouble())
                        {
                            lnProductId = productid.toInt();
                            //                                        qDebug()<<"productid : "<<lnProductId<<endl;
                        }
                    }
                    else
                    {
                        lnProductId = 0;
                    }

                    if(obj2.contains("release_ID"))
                    {
                        QJsonValue releaseid = obj2.take("release_ID");
                        if(releaseid.isDouble())
                        {
                            relid = releaseid.toInt();
//                                                                    qDebug()<<"releaseid : "<<relid<<endl;
                        }
                    }
                    else
                    {
                        relid = 0;
                    }

                    if(obj2.contains("category_ID"))
                    {
                        categoryid = obj2.take("category_ID");
                        if(categoryid.isDouble())
                        {
                            cateid = categoryid.toInt();
                            //                                        qDebug()<<"categoryid : "<<cateid<<endl;
                            it = jsonData->classElementNumMap.find(cateid);
                            if(it != jsonData->classElementNumMap.end())
                            {
                                jsonData->classElementNumMap[cateid] = it.value()+1;
                            }
                            else
                            {
                                jsonData->classElementNumMap.insert(cateid,1);
                            }
                        }
                    }
                    else
                    {
                        cateid = 0;
                    }

                    if(obj2.contains("product_name"))
                    {
                        QJsonValue productname = obj2.take("product_name");
                        if(productname.isString())
                        {
                            proname = productname.toString();
                        }
                    }
                    else
                    {
                        proname = "";
                    }

                    if(obj2.contains("vendor_name"))
                    {
                        QJsonValue vendorname = obj2.take("vendor_name");
                        if(vendorname.isString())
                        {
                            venname = vendorname.toString();
                            //                                        qDebug()<<"vendorname : "<<venname<<endl;
                        }
                    }
                    else
                    {
                        venname = "";
                    }

                    if(obj2.contains("icon_url"))
                    {
                        QJsonValue iconurl = obj2.take("icon_url");
                        if(iconurl.isString())
                        {
                            icourl = iconString + iconurl.toString();
                            //                                        qDebug()<<"icon_url : "<<icourl<<endl;
                        }
                    }
                    else
                    {
                        qDebug()<<lnProductId<<" "<<relid;
                        icourl = "";
                    }

                    if(obj2.contains("url"))
                    {
                        QJsonValue url = obj2.take("url");
                        if(url.isString())
                        {
                            prourl = url.toString();
                            //                                        qDebug()<<"url : "<<prourl<<endl;
                        }
                    }
                    else
                    {
                        prourl = "";
                    }

                    if(obj2.contains("product_description"))
                    {
                        QJsonValue productdescription = obj2.take("product_description");
                        if(productdescription.isString())
                        {
                            prodesc = productdescription.toString();
                        }
                    }
                    else
                    {
                        prodesc = "";
                    }

                    if(obj2.contains("executable_file"))
                    {
                        QJsonValue exe = obj2.take("executable_file");
                        if(exe.isString())
                        {
                            exefile = exe.toString();
                        }
                    }
                    else
                    {
                        exefile = "";
                    }

                    if(obj2.contains("package_name"))
                    {
                        QJsonValue name = obj2.take("package_name");
                        if(name.isString())
                        {
                            packname = name.toString();
                        }
                    }
                    else
                    {
                        packname = "";
                    }

                    if(obj2.contains("release_grade"))
                    {
                        QJsonValue releasegrade = obj2.take("release_grade");
                        if(releasegrade.isDouble())
                        {
                            relcate = releasegrade.toInt();
                            //                                        qDebug()<<"productgrade : "<<relcate<<endl;
                        }
                    }
                    else
                    {
                        relcate = 0;
                    }

                    if(obj2.contains("grade_count"))
                    {
                        QJsonValue gradecount = obj2.take("grade_count");
                        if(gradecount.isDouble())
                        {
                            gracount = gradecount.toDouble();
                        }

                        if(gracount >5 || gracount <0)
                        {
                            gracount = 0;
                        }
                    }
                    else
                    {
                        gracount = 0;
                    }
//                    qDebug()<<"gracount : "<<gracount<<endl;
//                    qDebug()<<"icourl : "<<icourl<<endl;
//                    qDebug()<<"packname ::::::: "<<packname<<endl;
//                    if(y<10)
//                    {
//                        icourl = QString("%1%2%3").arg("http://k2.jsqq.net/uploads/allimg/1705/7_170524143440_").arg(y+1).arg(".jpg");
//                        y++;
//                    }
//                    else
//                    {
//                        icourl = QString("%1%2%3").arg("http://k2.jsqq.net/uploads/allimg/1705/7_170524143440_").arg(1).arg(".jpg");
//                        y = 1;
//                    }

                    if(lnProductId != 0 && cateid != 0)
                        jsonData->classStrMap.insert(lnProductId,CLASSSTRUCT(cateid,relid,1,icourl,proname,gracount,prodesc,exefile,packname));
                }
            }
        }
        emit productIsOk();
    }
}

void JSONFUNC::getUpdateRelease(QJsonObject obj)
{    
//    qDebug()<<__FUNCTION__;
    if(obj.contains("releases"))
    {
        QJsonValue test = obj.take("releases");

        if(test.isArray())
        {
            QJsonArray str = test.toArray();
            int proid = 0;
            int relid = 0;
            QString ver;
            int packsize = 0;
            QString chaglog;
            QString name;
            QString downurl;
            QString icourl;
            QString exefile;
            QString packname;

            for(int i = 0;i < str.size();i++ )
            {
                QJsonValue value = str.at(i);

                if(value.isObject())
                {
                    QJsonObject obj2 = value.toObject();

                    if(obj2.contains("product_ID"))
                    {
                        QJsonValue productid = obj2.take("product_ID");
                        if(productid.isDouble())
                        {
                            proid = productid.toInt();
                        }
                    }
                    else
                    {
                        proid = 0;
                    }

                    if(obj2.contains("ID"))
                    {
                        QJsonValue releaseid = obj2.take("ID");
                        if(releaseid.isDouble())
                        {
                            relid = releaseid.toInt();
                        }
                    }
                    else
                    {
                        relid = 0;
                    }

                    if(obj2.contains("product_name"))
                    {

                        QJsonValue releasename = obj2.take("product_name");
                        if(releasename.isString())
                        {
                            name = releasename.toString();
                        }
                    }
                    else
                    {
                        name = "";
                    }

                    if(obj2.contains("version"))
                    {
                        QJsonValue version = obj2.take("version");
                        if(version.isString())
                        {
                            ver = version.toString();
                        }
                    }
                    else
                    {
                        ver = "";
                    }

                    if(obj2.contains("changelog"))
                    {
                        QJsonValue changelog = obj2.take("changelog");
                        if(changelog.isString())
                        {
                            chaglog = changelog.toString();
                        }
                    }
                    else
                    {
                        chaglog = "";
                    }

                    if(obj2.contains("package_size"))
                    {
                        QJsonValue packagesize = obj2.take("package_size");
                        if(packagesize.isDouble())
                        {
                            packsize = packagesize.toDouble();
                        }
                    }
                    else
                    {
                        packsize = 0;
                    }

                    if(obj2.contains("icon_url"))
                    {
                        QJsonValue iconurl = obj2.take("icon_url");
                        if(iconurl.isString())
                        {
                            icourl = iconString + iconurl.toString();
                        }
                    }
                    else
                    {
                        icourl = "";
                    }

                    if(obj2.contains("download_url"))
                    {
                        QJsonValue downloadurl = obj2.take("download_url");
                        if(downloadurl.isString())
                        {
                            downurl = downloadurl.toString();
                        }
                    }
                    else
                    {
                        downurl = "";
                    }

                    if(obj2.contains("executable_file"))
                    {
                        QJsonValue exe = obj2.take("executable_file");
                        if(exe.isString())
                        {
                            exefile = exe.toString();
                        }
                    }
                    else
                    {
                        exefile = "";
                    }

                    if(obj2.contains("package_name"))
                    {
                        QJsonValue name = obj2.take("package_name");
                        if(name.isString())
                        {
                            packname = name.toString();
                        }
                    }
                    else
                    {
                        packname = "";
                    }

//                    qDebug()<<"productname : "<<name<<endl;
                    qDebug()<<"proid : "<<proid<<endl;
//                    qDebug()<<"chaglog : "<<chaglog<<endl;
                    qDebug()<<"packsize :: "<<packsize<<endl;
//                    qDebug()<<"icourl : "<<icourl<<endl;
//                    qDebug()<<"downurl : "<<downurl<<endl;
                    qDebug()<<"ver : "<<ver<<endl;

                    if(proid != 0)
                        jsonData->updateStrMap.insert(relid,UPDATESTRUCT(proid,ver,icourl,name,chaglog,downurl,packsize,exefile,packname));
                }
            }
//            qDebug() << "updateStrMap == " << jsonData->updateStrMap.count();
            emit updateIsOk();
        }
    }
}

void JSONFUNC::getRecommend(QJsonObject obj)
{
    if(obj.contains("recommend"))
    {
        QJsonValue test = obj.take("recommend");
        if(test.isArray())
        {
            QJsonArray str = test.toArray();
            int id = 0;
            int priority = 0;
            for(int i = 0;i < str.size();i++ )
            {
                QJsonValue value = str.at(i);

                if(value.isObject())
                {
                    QJsonObject obj2 = value.toObject();

                    if(obj2.contains("ID"))
                    {
                        QJsonValue num = obj2.take("ID");
                        if(num.isDouble())
                        {
                            id = num.toInt();
                        }
                    }
                    else
                    {
                        id = 0;
                    }

                    if(obj2.contains("priority"))
                    {
                        QJsonValue pri = obj2.take("priority");
                        if(pri.isDouble())
                        {
                            priority = pri.toInt();
                        }
                    }
                    else
                    {
                        priority = 0;
                    }

                    if(id != 0)
                        jsonData->recommendMap[id] = priority;
//                    qDebug()<<"recommend.id  ==  "<<id<<"  recommend.value  ==  "<<priority<<endl;
                }
            }
        }
        emit recommendIsOk();
    }
}

void JSONFUNC::getScreenImage(QJsonObject obj)
{
    if(obj.contains("ScreenImage"))
    {
        QJsonValue test = obj.take("ScreenImage");
        if(test.isArray())
        {
            QJsonArray str = test.toArray();
            int size = str.size();

            for(int i = 0;i < size;i++ )
            {
                QJsonValue value = str.at(i);

                if(value.isObject())
                {
                    int id = 0;
                    int proId = 0;
                    int relId = 0;
                    QString imageUrl;
                    QJsonObject obj2 = value.toObject();

                    if(obj2.contains("ID"))
                    {
                        QJsonValue num = obj2.take("ID");
                        if(num.isDouble())
                        {
                            id = num.toInt();
                        }
                    }
                    else
                    {
                        id = 0;
                    }

                    if(obj2.contains("product_ID"))
                    {
                        QJsonValue num = obj2.take("product_ID");
                        if(num.isDouble())
                        {
                            proId = num.toInt();
                        }
                    }
                    else
                    {
                        proId = 0;
                    }

                    if(obj2.contains("release_ID"))
                    {
                        QJsonValue num = obj2.take("release_ID");
                        if(num.isDouble())
                        {
                            relId = num.toInt();
                        }
                    }
                    else
                    {
                        relId = 0;
                    }

                    if(obj2.contains("image_url"))
                    {
                        QJsonValue url = obj2.take("image_url");
                        if(url.isString())
                        {
                            imageUrl = url.toString();
                        }
                    }
                    else
                    {
                        imageUrl = "";
                    }

                    if(proId != 0 && id != 0)
                        jsonData->screenImageMap.insert(proId,SCREENIMAGE(id,relId,imageUrl));
                }
            }
        }
    }
}

void JSONFUNC::getComment(QJsonObject obj)
{
    if(obj.contains("releases"))
    {
        QJsonValue test = obj.take("releases");
        if(test.isArray())
        {
            QJsonArray str = test.toArray();
            int size = str.size();

            for(int i = 0;i < size;i++ )
            {
                QJsonValue value = str.at(i);

                if(value.isObject())
                {
                    int id = 0;
                    int proId = 0;
                    int relId = 0;
                    int userId = 0;
                    QString commentText;
                    int commentGrade;
                    QString commentDate;
                    QJsonObject obj2 = value.toObject();

                    if(obj2.contains("ID"))
                    {
                        QJsonValue num = obj2.take("ID");
                        if(num.isDouble())
                        {
                            id = num.toInt();
                        }
                    }
                    else
                    {
                        id = 0;
                    }

                    if(obj2.contains("product_ID"))
                    {
                        QJsonValue num = obj2.take("product_ID");
                        if(num.isDouble())
                        {
                            proId = num.toInt();
                        }
                    }
                    else
                    {
                        proId = 0;
                    }

                    if(obj2.contains("release_ID"))
                    {
                        QJsonValue num = obj2.take("release_ID");
                        if(num.isDouble())
                        {
                            relId = num.toInt();
                        }
                    }
                    else
                    {
                        relId = 0;
                    }

                    if(obj2.contains("user_ID"))
                    {
                        QJsonValue num = obj2.take("user_ID");
                        if(num.isDouble())
                        {
                            userId = num.toInt();
                        }
                    }
                    else
                    {
                        userId = 0;
                    }

                    if(obj2.contains("comment_text"))
                    {
                        QJsonValue text = obj2.take("comment_text");
                        if(text.isString())
                        {
                            commentText = text.toString();
                        }
                    }
                    else
                    {
                        commentText = "";
                    }

                    if(obj2.contains("comment_grade"))
                    {
                        QJsonValue num = obj2.take("comment_grade");
                        if(num.isDouble())
                        {
                            commentGrade = num.toInt();
                        }
                    }
                    else
                    {
                        commentGrade = 0;
                    }

                    if(obj2.contains("comment_date"))
                    {
                        QJsonValue date = obj2.take("comment_date");
                        if(date.isString())
                        {
                            commentDate = date.toString();
                        }
                    }
                    else
                    {
                        commentDate = "";
                    }

                    if(proId != 0 && id != 0)
                    jsonData->commentMap.insert(relId,COMMENT(id,relId,userId,commentText,commentGrade,commentDate));
                }
            }
        }
    }
}
