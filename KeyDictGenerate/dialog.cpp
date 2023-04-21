#include "dialog.h"
#include "ui_dialog.h"
#include <qpainter.h>
#include <qpixmap.h>
#include <qicon.h>
#include <qstringlist.h>
#include <QDebug>
#include <qfile.h>

int MinLen = 8, MaxLen = 8;
QString OverlapStr = "0123456789";

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/new/Pic/Scr/1.ico"));
    setWindowTitle("KeyDictGenerate    --Huixingzi");
    setFixedSize(400,700);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *)
{
    //设置背景
    QPainter *painter = new QPainter(this);
    QPixmap pic = QPixmap(":/new/Pic/Scr/1676900246274.jpg");

    painter->drawPixmap(0,0,this->width(),this->height(),pic);
}

//递归字符串列表，将各组合写入文件中，由于需要频繁打开文件，需优化，OrgList 为字符串列表， num 为该列表数量， 需要szfrontkey向下传递字符串列表,随意定义即可，无实义
void GenerateDict(QStringList OrgList, int num, QStringList szfrontKey){
    if (num > 0)
    {
        for (int i = 0; i < OrgList.count(); ++i)
        {
            QStringList EachKey;
            EachKey.append(szfrontKey);
            EachKey.append(OrgList.at(i));

            QString CurStr = EachKey.join("");
            if (CurStr.count() > MaxLen) continue;

            if (CurStr.count() >= MinLen){
                QFile file("./pwd.txt");
                if(!file.open(QIODevice::WriteOnly|QIODevice::Append)) qDebug()<<"文件打开失败";

                QTextStream in(&file);
                in << CurStr << "\n";
                file.close();
            }

            GenerateDict(OrgList, num - 1, EachKey);
        }
    }
}

//整理字符串列表，将大于16字节去除
void ArrangeList(QStringList &StrList)
{
    for (int i = 0; i < StrList.count(); ++i)
    {
        if (StrList.at(i).count() > 16) StrList.removeAt(i);
    }
}


//写重叠殊数字组合, NumOfEachNum为每个数字最大重叠数量，如“11”重叠2次，重叠0次退出
void writeOverlapNum()
{
    QStringList StrList;

    for (int OverlapNum = 2; OverlapNum <= 3; ++OverlapNum)
    {
        for (int i = 0; i < OverlapStr.count(); ++i) //重叠的元素
        {
            QString str;
            for (int j = 0; j < OverlapNum; ++j) //添加2/3重叠数元素进列表，2，3 重叠数可随意组合出更高重叠数密码，且组合数量较少
            {
                str.append(OverlapStr.at(i));
            }
            StrList.append(str);
        }
    }

    QStringList szfrontKey;
    GenerateDict(StrList, StrList.count(), szfrontKey);
}

//写入连续字符
void WriteSeriesChar(){
    //连续数字
    QStringList strlist;
    for (int i = 2; i <= 3; ++i)  //同上， 连续2，3次可以组合出更高连续次
    {
        for (unsigned char byte = '1'; byte <= '9'; ++byte)
        {
            if ((byte + i - 1) > '9') break;

             QString str;
             for (int j = 0; j < i; ++j)
             {
                 str.append(QString(byte + j));
             }
             strlist.append(str);
        }
    }

    QStringList szfrontKey;
    GenerateDict(strlist, strlist.count(), szfrontKey);

    //连续大写字符
    QStringList strlist1;
    for (int i = 2; i <= 3; ++i)  //同上， 连续2，3次可以组合出更高连续次
    {
        for (unsigned char byte = 'A'; byte <= 'Z'; ++byte)
        {
            if ((byte + i - 1) > 'Z') break;

             QString str;
             for (int j = 0; j < i; ++j)
             {
                 str.append(QString(byte + j));
             }
             strlist1.append(str);
        }
    }

    QStringList szfrontKey1;
    GenerateDict(strlist1, strlist1.count(), szfrontKey1);

    //连续小写字符
    QStringList strlist2;
    for (int i = 2; i <= 3; ++i)  //同上， 连续2，3次可以组合出更高连续次
    {
        for (unsigned char byte = 'a'; byte <= 'z'; ++byte)
        {
            if ((byte + i - 1) > 'z') break;

             QString str;
             for (int j = 0; j < i; ++j)
             {
                 str.append(QString(byte + j));
             }
             strlist2.append(str);
        }
    }

    QStringList szfrontKey2;
    GenerateDict(strlist2, strlist2.count(), szfrontKey2);
}

///生成字典
void Dialog::on_pushButton_2_clicked()
{
    ui->pushButton_2->setWindowTitle("生成中");

    MinLen = ui->lineEdit_5->text().toInt();
    MaxLen = ui->lineEdit_6->text().toInt();

    OverlapStr = ui->lineEdit_9->text();

    QStringList StrList;

    QString Name = ui->lineEdit->text();
    if (!Name.isEmpty()) {
        QStringList NameList = Name.split(" ");
        QString EasyName;
        for (int i = 0; i < NameList.count(); ++i)
        {
            EasyName.append(NameList.at(i).at(0));
        }

        StrList << EasyName.toLower();
        StrList << EasyName.toUpper();

        StrList << Name;
    }

    QString Birthday = ui->lineEdit_2->text();
    if (!Birthday.isEmpty()) {
        StrList << Birthday;
        QStringList BirthdayList = Birthday.split(" ");
        StrList.append(BirthdayList);
    }

    QString Tel = ui->lineEdit_3->text();
    if (!Tel.isEmpty()) StrList << Tel;

    QString QQ = ui->lineEdit_4->text();
    if (!QQ.isEmpty()) StrList << QQ;

    QString DoMain = ui->lineEdit_7->text();
    if (!DoMain.isEmpty()) StrList << DoMain;

    QString UserName = ui->lineEdit_8->text();
    if (!UserName.isEmpty()) StrList << UserName;

    QString KeyWord = ui->textEdit->toPlainText();
    if (!KeyWord.isEmpty()) StrList.append(KeyWord.split("\n"));

    //处理列表,清除非法字符串
    ArrangeList(StrList);

    QStringList szfrontKey;
    GenerateDict(StrList, StrList.count(), szfrontKey); //相关信息

    writeOverlapNum(); //重叠字符

    WriteSeriesChar();//连续字符

    ui->pushButton_2->setWindowTitle("生成字典");
}

//清空
void Dialog::on_pushButton_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->textEdit->clear();

    QFile file("./pwd.txt");
    if(!file.open(QIODevice::WriteOnly)) qDebug()<<"文件打开失败";
    file.close();
}
