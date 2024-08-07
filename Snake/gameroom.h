#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <QWidget>
#include <QSound>
#include <QList>
#include <QRectF>

enum class SnakeDirect  //枚举出蛇移动的方向
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};

class gameroom : public QWidget
{
    Q_OBJECT
public:
    explicit gameroom(QWidget *parent = nullptr);

    //绘图
    void paintEvent(QPaintEvent *event);

    // 蛇的上下左右移动
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    // 游戏是否结束
    bool checkFail();

    // 创建食物
    void createNewFood();

    //设置蛇的移动时间
    void setTimeout(int timeout) {moveTimeout = timeout;}

private:
    const int kSnakeNodeWidth = 20; //  蛇的宽度
    const int kSnakeNodeHeight = 20; // 蛇的高度
    const int kDefaultTimeout = 200; // 蛇默认的移动速度

    QList<QRectF> snakeList; //  QRectF表示矩形，这里表示贪吃蛇链表
    QRectF FoodRect;    //表示食物节点
    SnakeDirect moveDirect = SnakeDirect::UP;   //  枚举蛇默认的移动方向是向上的
    QTimer *timer; // 定时器
    bool isGameStart = false;   // 表示是否开始游戏
    QSound * sound;

    int moveTimeout = kDefaultTimeout;  //  蛇的移动时间

signals:

};

#endif // GAMEROOM_H
