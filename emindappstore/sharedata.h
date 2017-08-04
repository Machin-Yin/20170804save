#ifndef SHAREDATA_H
#define SHAREDATA_H
#include <QString>
#include <QMap>

//分类项结构体
struct CLASSSTRUCT{
    int category;
    int releaseId;
    QString proImage;
    QString proName;
    double proStar;
    QString proDescription;
    CLASSSTRUCT(int cate,int releaseid,const QString & image,const QString & name,double star,const QString & desc):
        category(cate),releaseId(releaseid),proImage(image),proName(name),proStar(star),proDescription(desc){}
};

//更新数据结构体
struct UPDATESTRUCT{
    int productId;
//    int releaseId;
    QString version;
    QString proImage;
    QString proName;
    QString changeLog;
    QString downloadUrl;
    int packageSize;
    UPDATESTRUCT(int id,QString ver,const QString &image,const QString &name,const QString & changelog,const QString & downloadurl,int packagesize):
        productId(id),version(ver),proImage(image),proName(name),changeLog(changelog),downloadUrl(downloadurl),packageSize(packagesize){}
};

//产品截图结构体
struct SCREENIMAGE{
    int imageId;
    int releaseId;
    QString imageUrl;
    SCREENIMAGE(int imageid,int releaseid,const QString & imageurl):
        imageId(imageid),releaseId(releaseid),imageUrl(imageurl){}
};

struct COMMENT{
    int commentId;
    int releaseId;
    int userId;
    QString commentText;
    int commentGrade;
    QString CommentDate;
    COMMENT(int commentid,int releaseid,int userid,const QString & commenttext,int commentgrade,const QString & commentdate):
        commentId(commentid),releaseId(releaseid),userId(userid),commentText(commenttext),commentGrade(commentgrade),CommentDate(commentdate){}
};

//int 代表分类项编号，QString代表分类名
typedef QMap<int,QString> CATEGORYMAP;

//int 代表product id
typedef QMap<int,CLASSSTRUCT> CLASSSTRUCTMAP;

//第一个int代表分类id，第二个代表每个分类有多少软件元素
typedef QMap<int,int> ELEMENTNUMBERMAP;

//int 代表product id
typedef QMap<int,UPDATESTRUCT> UPDATESTRUCTMAP;

//int 代表product id，第二个int代表优先级
typedef QMap<int,int> RECOMMENDMAP;

//int 代表product id
typedef QMap<int,SCREENIMAGE> SCREENIMAGEMAP;

//int 代表product id
typedef QMap<int,COMMENT> COMMENTMAP;

class ShareData
{
public:
    CATEGORYMAP categoryMap;    //软件分类名map
    CLASSSTRUCTMAP classStrMap;     //软件分类map
    ELEMENTNUMBERMAP classElementNumMap;    //每个分类包含软件数map
    UPDATESTRUCTMAP updateStrMap;   //更新数据map
    RECOMMENDMAP recommendMap;  //热门推荐map
    SCREENIMAGEMAP screenImageMap;  //产品截图map
    COMMENTMAP commentMap;
};

#endif // SHAREDATA_H
