#include "gameselect.h"
#include <QIcon>
#include <QPainter>
#include "gamehall.h"
#include <QPixmap>
#include <QPushButton>
#include <QFont>
#include "gameroom.h"
#include <QSound>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>

GameSelect::GameSelect(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(800, 800);   //  使用fixedsize设置窗口大小，并且是固定窗口，
    this->setWindowIcon(QIcon(":/picture/icon.png"));
    this->setWindowTitle("关卡选择大厅页面");

    //  返回页面按钮
    QPushButton * backBtn = new QPushButton(this);
    backBtn->setIcon(QIcon(":/picture/back.jpg"));

    backBtn->setFixedSize(70, 70);  //设置按钮尺寸的大小
    backBtn->setIconSize(QSize(70,70)); //  设置的大小
    backBtn->move(this->width()*0.9, this->height()*0.9);
    backBtn->setStyleSheet("QPushButton{border:0px;}" //   去除按钮边框
                            "QPushButton:hover{color:red;}");

    connect(backBtn, &QPushButton::clicked, [=](){
        this->close();
        GameHall * gamehall = new GameHall;
        gamehall->show();
        QSound::play(":/picture/clicked.wav");
    });
    //关卡选择
    QFont font("华文行楷",23);
    gameroom * gameRoom = new gameroom();

    QPushButton * simeleBtn = new QPushButton(this);
    simeleBtn->setText("简单模式");
    simeleBtn->move(this->width()*0.39, this->height()* 0.3);
    simeleBtn->setFixedSize(150, 60);
    simeleBtn->setFont(font);
    simeleBtn->setStyleSheet("QPushButton{border:0px;}" //   去除按钮边框
                             "QPushButton:hover{color:red;}");    //  鼠标放上去字体颜色为红色
    connect(simeleBtn, &QPushButton::clicked,[=](){
        this->close();
        gameRoom->show();
        QSound::play("/picture/clicked.wav");

        gameRoom->setTimeout(300);  //  蛇移动速度为300
    });

    QPushButton * normalBtn = new QPushButton(this);
    normalBtn->setText("正常模式");
    normalBtn->move(this->width()*0.39, this->height()* 0.4);
    normalBtn->setFixedSize(150, 60);
    normalBtn->setFont(font);
    normalBtn->setStyleSheet("QPushButton{border:0px;}" //   去除按钮边框
                             "QPushButton:hover{color:red;}");    //  鼠标放上去字体颜色为红色
    connect(normalBtn, &QPushButton::clicked,[=](){
        this->close();
        gameRoom->show();
        QSound::play("/picture/clicked.wav");

        gameRoom->setTimeout(200);
    });

    QPushButton * hardBtn = new QPushButton(this);
    hardBtn->setText("困难模式");
    hardBtn->move(this->width()*0.39, this->height()* 0.5);
    hardBtn->setFixedSize(150, 60);
    hardBtn->setFont(font);
    hardBtn->setStyleSheet("QPushButton{border:0px;}" //   去除按钮边框
                             "QPushButton:hover{color:red;}");    //  鼠标放上去字体颜色为红色
    connect(hardBtn, &QPushButton::clicked,[=](){
        this->close();
        gameRoom->show();
        QSound::play("/picture/clicked.wav");

        gameRoom->setTimeout(100);
    });

    QPushButton * hisBtn = new QPushButton(this);
    hisBtn->setText("历史战绩");
    hisBtn->move(this->width()*0.39, this->height()* 0.6);
    hisBtn->setFixedSize(150, 60);
    hisBtn->setFont(font);
    hisBtn->setStyleSheet("QPushButton{border:0px;}" //   去除按钮边框
                             "QPushButton:hover{color:red;}");    //  鼠标放上去字体颜色为红色

    connect(hisBtn, &QPushButton::clicked,[=](){
        QWidget * widget = new QWidget;
        widget->setWindowTitle("历史战绩");
        widget->setFixedSize(500, 300);

        QTextEdit *edit = new QTextEdit(widget);
        edit->setFont(font);
        edit->setFixedSize(500, 300);
        QFile file(":/picture/score.txt");
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        int data = in.readLine().toInt();//由于读取到的是字符串所以要转换成整型
        edit->append("得分为：");
        edit->append(QString::number(data));

        widget->show();//显示窗口

    });

}

void GameSelect::paintEvent(QPaintEvent *event)
{
    QPixmap pix(":/picture/game_select .jpg"); //  导入要实例化的图片
    QPainter painter(this); //  实例化对象
    painter.drawPixmap(0, 0, this->width(), this->height(),pix);

}
