#include "gameroom.h"
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <gameselect.h>


gameroom::gameroom(QWidget *parent) : QWidget(parent)
{
    //返回上一个窗口
    QPushButton * backBtn = new QPushButton(this);
    backBtn->setIcon(QIcon(":/picture/back.jpg"));

    backBtn->setFixedSize(50, 50);  //设置按钮尺寸的大小
    backBtn->setIconSize(QSize(50,50)); //  设置的大小
    backBtn->move(750, 750);
    backBtn->setStyleSheet("QPushButton{border:0px;}" //   去除按钮边框
                            "QPushButton:hover{color:red;}");

    connect(backBtn, &QPushButton::clicked, [=](){
        this->close();
        GameSelect * gameselect = new GameSelect;
        gameselect->show();
        QSound::play(":/picture/clicked.wav");
    });



    //本次窗口页面
    this->setFixedSize(800, 800);   //  使用fixedsize设置窗口大小，并且是固定窗口，
    this->setWindowIcon(QIcon(":/picture/icon.png"));   //  设置窗口图标
    this->setWindowTitle("游戏页面");    //设置窗口标题



    //初始化贪吃蛇(链表)    QRectF表示矩形的类，里面的参数就是矩形的宽高和(x, y)坐标
    snakeList.push_back(QRectF(this->width() * 0.5, this->height() * 0.5,
                               kSnakeNodeWidth, kSnakeNodeHeight));
    moveUp();
    moveUp();

    //创建食物
    createNewFood();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        int cnt = 1;    //统计和食物相交的次数

        if(snakeList.empty())
        {
            qDebug() << "connect(timer, &QT";
        }
        if(snakeList.front().intersects(FoodRect))  //  intersects是QT提供的判断两个矩形是否相交的方法
        {
            createNewFood();    //  相交在增加一次食物


            cnt++;
            qDebug()<<"食物";
        }
        while (cnt--)
        {
            qDebug()<<"移动1";
            switch (moveDirect)
            {
            case SnakeDirect::UP:
                moveUp();
                qDebug()<<"移动2";
                break;
            case SnakeDirect::DOWN:
                moveDown();
                break;
            case SnakeDirect::LEFT:
                moveLeft();
                break;
            case SnakeDirect::RIGHT:
                moveRight();
                break;
            }
        }
        //  蛇移动，头节点移动，后面的节点跟上，但是最后一个节点是删除在添加
        snakeList.pop_back();//删除最后一个节点
        update();//更新链表
        qDebug()<<"更新链表";
    });

    //开始游戏，暂停游戏
    QPushButton* strBtn = new QPushButton(this);
    QPushButton* stoBtn = new QPushButton(this);
    QPushButton* ExitBtn = new QPushButton(this);
    strBtn->move(60, 615);
    stoBtn->move(60, 680);
    ExitBtn->move(60, 745);
    strBtn->setText("开始游戏P");
    strBtn->setShortcut((QKeySequence(Qt::Key_P)));
    stoBtn->setText("暂停游戏k");
    stoBtn->setShortcut((QKeySequence(Qt::Key_K)));
    ExitBtn->setText("退出游戏ESC");
    ExitBtn->setShortcut((QKeySequence(Qt::Key_Escape)));
    //点击退出游戏会出现消息盒子
    QMessageBox *msg = new QMessageBox(this);
    QPushButton *ok = new QPushButton("ok");
    QPushButton *cancel = new QPushButton("cancel");
    msg->addButton(ok, QMessageBox::AcceptRole);
    msg->addButton(cancel, QMessageBox::RejectRole);
    msg->setWindowTitle("退出游戏");
    msg->setText("是否确认退出游戏");

    connect(ExitBtn, &QPushButton::clicked,[=](){   //  显示消息盒子的信号槽
        msg->show();
        QSound::play("/picture/clicked.wav");
        msg->exec();
        if(msg->clickedButton() == ok)
        {
            this->close();
        }
        else
        {
            msg->close();
        }
    });

    QFont font("楷体",20);
    stoBtn->setFont(font);
    strBtn->setFont(font);
    ExitBtn->setFont(font);
//  开始游戏信号槽
    connect(strBtn, &QPushButton::clicked, [=](){
       isGameStart = true;
       timer->start(moveTimeout);
       sound = new QSound(":/picture/Trepak.wav");  //QSound是
       sound->play();   //  播放
       sound->setLoops(-1);
    });
//  暂停游戏信号槽
    connect(stoBtn, &QPushButton::clicked, [=](){
       isGameStart = false;
       timer->start(kDefaultTimeout);
       timer->stop();
       sound->stop();
    });
    //  方向控制
    QFont ft("微软雅黑",20);
    QPushButton *up = new QPushButton(this);
    up->move(592, 635);
    up->setText("W");
    up->setShortcut((QKeySequence(Qt::Key_W))); // 快捷键
    up->setStyleSheet("QPushButton{border:0px;}");
    up->setFont(ft);

    QPushButton *down = new QPushButton(this);
    down->move(600, 675);
    down->setText("S");
    down->setShortcut((QKeySequence(Qt::Key_S)));
    down->setStyleSheet("QPushButton{border:0px;}");
    down->setFont(ft);

    QPushButton *left = new QPushButton(this);
    left->move(550,675);
    left->setText("A");
    left->setShortcut((QKeySequence(Qt::Key_A)));
    left->setStyleSheet("QPushButton{border:0px;}");
    left->setFont(ft);
    QPushButton *right = new QPushButton(this);
    right->move(650,675);
    right->setText("D");
    right->setShortcut((QKeySequence(Qt::Key_D)));
    right->setStyleSheet("QPushButton{border:0px;}");
    right->setFont(ft);

    //  按钮进行关联
    connect(up, &QPushButton::clicked, [=](){
        if(moveDirect != SnakeDirect::DOWN)
        {
            moveDirect = SnakeDirect::UP;
        }
    });

    connect(down, &QPushButton::clicked, [=](){
        if(moveDirect != SnakeDirect::UP)
        {
            moveDirect = SnakeDirect::DOWN;
        }
    });

    connect(left, &QPushButton::clicked, [=](){
        if(moveDirect != SnakeDirect::RIGHT)
        {
            moveDirect = SnakeDirect::LEFT;
        }
    });

    connect(right, &QPushButton::clicked, [=](){
        qDebug() << "1111111";
        if(moveDirect != SnakeDirect::LEFT)
        {
            moveDirect = SnakeDirect::RIGHT;
        }
    });
    int c = snakeList.size();
    QFile file(":/picture/score.txt");//    这是个样例，由于在qrc中存储的所以当退出程序会自动析构清除。
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        int num = c;
        out << num;
        file.close();
    }
}

void gameroom::paintEvent(QPaintEvent *event)   //绘图事件
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/picture/preview.png");//    游戏区域
    painter.drawPixmap(0, 0, 800, 600, pix);    //设置页面范围

    pix.load(":/picture/preview.jpg");//    显示操纵，战绩界面
    painter.drawPixmap(0, 600, 800, 200, pix);

    //  蛇头+蛇身+蛇尾
    //  蛇头
    if(moveDirect == SnakeDirect::UP)
    {
        pix.load(":/picture/up.png");
    }
    else if (moveDirect == SnakeDirect::DOWN)
    {
        pix.load(":/picture/down.png");
    }
    else if (moveDirect == SnakeDirect::LEFT)
    {
        pix.load(":/picture/left.png");
    }
    else if (moveDirect == SnakeDirect::RIGHT)
    {
        pix.load(":/picture/right.png");
    }
    auto snakeHead = snakeList.front();//获取蛇头节点

    painter.drawPixmap(snakeHead.x(), snakeHead.y(), snakeHead.width(), snakeHead.height(), pix);//进行绘制
    //  蛇身
    pix.load(":/picture/Bd.png");

    for (int i = 1; i < snakeList.size() - 1; i++) // 从一开始是去除蛇头，减一是去除蛇尾
    {
        auto node = snakeList.at(i); // 获得节点
        painter.drawPixmap(node.x(), node.y(), node.width(), node.height(), pix);
    }
    //  蛇尾
    auto snakeTail = snakeList.back();
    painter.drawPixmap(snakeTail.x(), snakeTail.y(), snakeTail.width(), snakeTail.height(), pix);

    //食物
    pix.load(":/picture/food.png");
    painter.drawPixmap(FoodRect.x(), FoodRect.y(), FoodRect.width()*1.5, FoodRect.height()*1.5, pix);

    //分数
    QPen pen;   //画笔
    pen.setColor(Qt::blue);    //黑色
    painter.setPen(pen);    //  画家使用画笔
    QFont font("方正舒体", 22);
    painter.setFont(font);
    painter.drawText(400, 700, QString("%1").arg(snakeList.size()));

    //绘制游戏是否失败
    if(checkFail())
    {
        pen.setColor(Qt::red);
        painter.setPen(pen);
        QFont font("方正舒体", 40);
        painter.setFont(font);
        painter.drawText(400, 700, QString("游戏失败"));
        timer->stop();
        QSound::play(":/picture/gameover.wav");
        sound->stop();
    }


    //向文件在写分数
    int c = snakeList.size();
    QFile file(":/picture/score.txt");//    这是个样例，由于在qrc中存储的所以当退出程序会自动析构清除。
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        int num = c;
        out << num;
        file.close();
    }
}

//  蛇是一个矩形，在移动的时候根据左上角和右下角进行定义
void gameroom::moveUp()
{
    QPointF leftTop;    //  左上角坐标
    QPointF rightBottom;    //  右下角坐标
    auto snakeNode = snakeList.front();  //  蛇头(list的头结点)
    int headX = snakeNode.x();  // 头结点的横坐标
    int headY = snakeNode.y();  // 头结点的纵坐标

    //  左上角
    if(headY < 0)   //  超出窗口顶部的范围
    {   //  向上移动纵坐标改变，横坐标不会改变
        leftTop = QPointF(headX, 600 - kSnakeNodeHeight);  //左上角的位置(点)就是我们定义游戏区域窗口的位置-蛇头(链表)的高度
    }
    else    //  没有超出
    {
        // 坐标为左手坐标，越向上Y越小，如果没有超出，那么现在左上角的高度是蛇头(链表)本身的高度减去它矩形的高度就得出坐标了
        leftTop = QPointF(headX, headY - kSnakeNodeHeight);
    }

    //  右下角(由于它是基于左上角来判定的所以不用判定是否穿墙)
    // 左上角点的坐标加上蛇头(链表)的高度与宽度，右下角在左上角的右下方基于坐标是变大的
    rightBottom = leftTop + QPointF(kSnakeNodeWidth, kSnakeNodeHeight); // 如果是基于左上角来定右下角那么右下角的大小永远是这样计算的

    snakeList.push_front(QRectF(leftTop, rightBottom));//   利用头插的方法将节点更新到链表当中
}

void gameroom::moveDown()
{
    QPointF leftTop;    //  左上角坐标
    QPointF rightBottom;    //  右下角坐标
    auto snakeNode = snakeList.front();  //  蛇头(list的头结点)
    int headX = snakeNode.x();  // 头结点的横坐标
    int headY = snakeNode.y();  // 头结点的纵坐标

    //左下角
    if(headY > 560)  //  当纵坐标的点大于窗口的高度说明这个点在窗口下面，那么蛇头也就跑了出去
    {
        leftTop = QPointF(headX, 0);    //  当左上角的纵坐标为0也就说明它穿墙了，从上面的墙跑了出来
    }
    else
    {
            leftTop = snakeNode.bottomLeft();   //  bottomLeft是QT自身提供的矩形左下角的坐标，当蛇进行移动的时候，左下角的坐标就是下一个左上角要移动到的坐标
    }

    //右下角和向上移动判定一致
    rightBottom = leftTop + QPointF(kSnakeNodeWidth, kSnakeNodeHeight);
    snakeList.push_front(QRectF(leftTop,rightBottom));  //  这个是作用是更新结点，如果不写会导致移动时节点无非更新而发生系统崩溃
}

void gameroom::moveLeft()
{
    qDebug() << "void gameroom::moveLeft()";
    QPointF leftTop;    //  左上角坐标
    QPointF rightBottom;    //  右下角坐标


    if(snakeList.empty())
    {
        qDebug() << "snakeList.empty()";
    }
    auto snakeNode = snakeList.front();  //  蛇头(list的头结点)
    int headX = snakeNode.x();  // 头结点的横坐标
    int headY = snakeNode.y();  // 头结点的纵坐标

    if(headX < 20)   //  穿墙
    {
        leftTop = QPointF(this->width()-kSnakeNodeWidth,headY); //穿墙之后的坐标
    }
    else // 在墙内
    {
        leftTop = QPointF(headX - kSnakeNodeWidth, headY);
    }
    rightBottom = leftTop + QPointF(kSnakeNodeWidth, kSnakeNodeHeight);
    snakeList.push_front(QRectF(leftTop,rightBottom));
}

void gameroom::moveRight()
{
    QPointF leftTop;    //  左上角坐标
    QPointF rightBottom;    //  右下角坐标
    auto snakeNode = snakeList.front();  //  蛇头(list的头结点)
    int headX = snakeNode.x();  // 头结点的横坐标
    int headY = snakeNode.y();  // 头结点的纵坐标

    if(headX > this->width()-20)   //  穿墙
    {
        leftTop = QPointF(0, headY);    //  穿墙后的坐标
    }
    else
    {
        leftTop = snakeNode.topRight(); //  获得矩形右上角的位置
    }

    rightBottom = leftTop + QPointF(kSnakeNodeWidth, kSnakeNodeHeight);
    snakeList.push_front(QRectF(leftTop,rightBottom));
}

bool gameroom::checkFail()  //  判断游戏结束，蛇碰到了自己
{
    for(int i = 0; i < snakeList.size(); i++) //    遍历整个蛇身和蛇头
    {
        for(int j = i + 1; j < snakeList.size(); j++) // 遍历蛇身没有蛇头
        {
            if(snakeList.at(i) == snakeList.at(j)) // .at表示什么
            {
                return true;
            }
        }
    }
    return false;
}

void gameroom::createNewFood()  //  创建食物
{
    FoodRect = QRectF(qrand() % (800 / kSnakeNodeWidth) * kSnakeNodeWidth,  //  随机节点的位置
                      qrand() % (600 / kSnakeNodeHeight) * kSnakeNodeHeight,//
                      kSnakeNodeWidth,//    食物的宽和高就是蛇节点的宽和高
                      kSnakeNodeHeight);
}
