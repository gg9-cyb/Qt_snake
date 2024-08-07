#include "gamehall.h"
#include "ui_gamehall.h"
#include <QPainter>
#include <QPaintEvent>
#include <QIcon>
#include <QPushButton>
#include <QFont>
#include <gameselect.h>
#include <QSound>//  模板没有，需要自身挂载到por文件中

GameHall::GameHall(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameHall)
{
    ui->setupUi(this);
    this->setFixedSize(800, 800);   //  使用fixedsize设置窗口大小，并且是固定窗口，
    //this->setGeometry();也能设置窗口大小，不过不是固定窗口，并且需要设置四个值
    this->setWindowIcon(QIcon(":/picture/icon.png"));   //  设置窗口图标
    this->setWindowTitle("贪吃蛇");    //设置窗口标题


    //  创建进入游戏按钮
    QPushButton *startBtn = new QPushButton(this);
    startBtn->setText("进入游戏");  //  设置按钮字体
    startBtn->move(this->width()*0.39, this->height()*0.5); //  设置按钮位置
    QFont font("方正舒体", 30, QFont::ExtraLight, false);   //  设置字体格式
    startBtn->setFont(font);    //  将样式付给按钮的格式
    startBtn->setStyleSheet("QPushButton{border:0px;}" //   去除按钮边框
                            "QPushButton:hover{color:red;}");    //  鼠标放上去字体颜色为红色

    GameSelect *gameSelect = new GameSelect();  //创建一个新窗口，这个串口为游戏大厅界面
    connect(startBtn, &QPushButton::clicked, [=](){
        gameSelect->setGeometry(this->geometry());  //  窗口的大小和这个大小一致
        this->hide();   //  关闭这个窗口
        gameSelect->show(); //  打开创建的窗口
        QSound::play("/picture/clicked.wav");
    });
}

GameHall::~GameHall()
{
    delete ui;
}

void GameHall::paintEvent(QPaintEvent *event)   //  绘图事件
{
    QPixmap pix(":/picture/game_hall.jpg"); //  导入要实例化的图片
    QPainter painter(this); //  实例化对象
    painter.drawPixmap(0, 0, this->width(), this->height(),pix);

}

