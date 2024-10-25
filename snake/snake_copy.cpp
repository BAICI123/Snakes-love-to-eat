#include "snake.h"
#include <QMessageBox>
#include <QString>
#include <QtDebug>
#include <windows.h>
#include <QThread>
#include <QStyle>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QApplication>
#include <QTime>

Snake::Snake(QWidget *parent) :
    QMainWindow(parent)
{
    //1.初始化成员变量和窗口设置

    score = 0;   //分数
    level = 1;   //等级

    this->setWindowTitle("斯内克");//主窗口标题
    this->resize((MAX_X + 7) * BLOCK_SIZE,MAX_Y * BLOCK_SIZE);//主窗口大小（之所以加7，一部分游戏，一部分显示文字）
    this->setFixedSize((MAX_X + 7) * BLOCK_SIZE,MAX_Y * BLOCK_SIZE); //固定大小

    //2.创建并设置开始按钮
    startGame = new QPushButton("startGame",this);  //父对象为w主窗口

    //按钮字体风格。背景浅绿，按钮边框圆角半径10px，文本粗体白色
    startGame->setStyleSheet("QPushButton{ background-color: rgb(106,192,212); border-radius: 10px; font: bold; color: white; }");

    //宋体15号
    QFont fontBtn("SimSun",15);
    startGame->setFont(fontBtn);

    //按钮位置及大小，位于游戏界面中间，大小160*80
    startGame->setGeometry(MAX_X * BLOCK_SIZE / 2 - 80,MAX_Y * BLOCK_SIZE / 2 - 40,160,80);

    //3.创建并设置游戏信息标签
    mLabel = new QLabel(this);  //父对象为 w 主窗口

    //信息栏位置及大小
    mLabel->setGeometry(MAX_X * BLOCK_SIZE,BLOCK_SIZE,6 * BLOCK_SIZE,(MAX_Y - 2) * BLOCK_SIZE);

    QFont fontView("SimSun",16);
    mLabel->setFont(fontView);
    mLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // 将十进制分数转换为QString类型
    QString viewText = QString::number (score,10);

    //使用html格式，防止使用多次setText文本被覆盖
    QString labelText = "<html><body>"
                        "<font size=\"4\">score: </font>" + viewText + "<br/>" +
                        "<font size=\"4\">level: </font>" + QString::number(level, 10) + "<br/><br/>" +
                        "<font size=\"4\">规则：</font><br/>" +
                        "<font size=\"2\">继续/上：箭头上<br/>" +
                        "继续/下：箭头下<br/>" +
                        "继续/左：箭头左<br/>" +
                        "继续/右：箭头右<br/>" +
                        "暂停/开始：空格<br/>"+
                        "shift:加速"
                        "</body></html>";
    mLabel->setText(labelText);


    init();         // 初始化游戏窗口
    drawBorder();
    initSnake();
    drawViewArea();

    //4.连接信号和槽函数
    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(snakeMoveSlots()));
    QObject::connect(startGame,SIGNAL(clicked()),this,SLOT(startGameSlots()));

}

Snake::~Snake()
{
}

 //画显示信息区域的边界
void Snake::drawViewArea()
{
    for(int x = MAX_X; x < MAX_X + 7; x++)
    {
        for(int y = 0; y < MAX_Y; y += MAX_Y - 1)
        {
            QLabel *label = new QLabel(this);
            label->setStyleSheet("background:gray");
            label->setGeometry(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1);
        }
    }

    for(int y = 1; y < MAX_Y - 1; y++)
    {
        QLabel *label = new QLabel(this);
        label->setStyleSheet("background:gray");
        label->setGeometry((MAX_X + 6) * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1);
    }

}

// 初始化游戏
void Snake::init()
{
    foodCount = 0;
    moveSpeed = 400;
    // 先把所有的label都初始化为NORMAL类型
    for(int x = 0; x < MAX_X; x++)
    {
        for(int y = 0; y < MAX_Y; y++)
        {
            //将游戏区域划分为小格并隐藏
            QLabel *label = new QLabel(this);
            label->setStyleSheet("background:gray");
            label->setGeometry(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1);
            label->hide();

            matrix[x][y] = new snakeNode;        //蛇节点
            matrix[x][y]->label = label;         //放到蛇节点中
            matrix[x][y]->type = NORMAL_LABEL;   //背景
            matrix[x][y]->x = x;                 //行
            matrix[x][y]->y = y;                 //列
        }
    }
}

// 画边界
void Snake::drawBorder()
{
    for(int x = 0; x < MAX_X; x++)
    {
        for(int y = 0; y < MAX_Y; y++)
        {
            if(x == 0 || y == 0 || x == MAX_X -1 || y == MAX_Y - 1)
            {
                matrix[x][y]->label->setStyleSheet("background:gray");
                matrix[x][y]->label->show();
                matrix[x][y]->type = BORDER_LABEL;
            }
        }
    }
}

// 初始化蛇
void Snake::initSnake()
{
    int snakeLen = 3;   // 蛇长
    int snakeX = 5;     // 蛇头初始化坐标
    int snakeY = 5;

    directionX = 1;     //初始移动方向向右
    directionY = 0;
    snake.clear();      // 清除蛇身
    // 将蛇结点追加到表尾，表尾即蛇头
    for(int x = snakeX; x < snakeX + snakeLen; x++)
    {
        //添加元素
        snake.append(matrix[x][snakeY]);
        //访问索引位置
        snake.at(snake.length() - 1)->x = x;
        snake.at(snake.length() - 1)->y = snakeY;
        matrix[x][snakeY]->type = SNAKE_LABEL;  //蛇身
        matrix[x][snakeY]->label->setStyleSheet("background:pink");
        matrix[x][snakeY]->label->show();
    }
}

// 移动操作
void Snake::moveSnake()
{
    //获取蛇头和蛇尾指针
    getHeadTail();
    //由键盘事件获得的方向来确定蛇头的位置，获取下一个位置的节点
    snakeNode *tempNode = matrix[head->x + directionX][head->y + directionY];
    tempNode->label->setStyleSheet("background:pink");
    tempNode->label->show();
    // 游戏结束检测，撞到自己和边界即游戏结束
    if(tempNode->type == BORDER_LABEL || tempNode->type == SNAKE_LABEL)
    {
        tempNode->label->setStyleSheet("background:gray");
        gameOver();     // 游戏结束
    }
    else
    {
        // 吃到食物
        if(tempNode->type == FOOD_LABEL)
        {
            showViewArea();
            tempNode->type = SNAKE_LABEL;
            snake.append(tempNode); // 追加到末尾，不隐藏蛇尾
            createFood();           // 生成新的食物
        }
        else
        {
            tempNode->type = SNAKE_LABEL;
            snake.append(tempNode);
            tail->label->hide();    // 将蛇尾隐藏
            tail->type = NORMAL_LABEL;
            // 删除表头，即蛇尾，QList中的操作，移除第一个元素
            snake.removeFirst();
        }
    }
}

// 显示分数和等级
void Snake::showViewArea()
{
    foodCount++;
    level = (foodCount / 10) + 1;   // 等级最高为8级
    if(level >= 8)
    {
        level = 8;
    }

    score += 1 * level;      // 计算分数

    QString viewText = QString::number (score,10);
    QString labelText = "<html><body>"
                        "<font size=\"4\">score: </font>" + viewText + "<br/>" +
                        "<font size=\"4\">level: </font>" + QString::number(level, 10) + "<br/><br/>" +
                        "<font size=\"4\">规则：</font><br/>" +
                        "<font size=\"2\">继续/上：箭头上<br/>" +
                        "继续/下：箭头下<br/>" +
                        "继续/左：箭头左<br/>" +
                        "继续/右：箭头右<br/>" +
                        "暂停/开始：空格<br/>" +
                        "shift:加速"
                        "</body></html>";
    mLabel->setText(labelText);
}

// 获取蛇头和蛇尾指针
void Snake::getHeadTail()
{
    // 蛇头位于表的尾部，即表头为蛇尾
    head = snake.at(snake.length() - 1);
    tail = snake.at(0);
}

void Snake::snakeMoveSlots()
{
    //移动速度
    if(accelerate){
        moveSpeed = 100 - level * 5;
    }
    else{
        moveSpeed = 50 * (9 - level);
    }
    // 开始移动
    timer.start(moveSpeed);
    moveSnake();
}

// 开始游戏
void Snake::startGameSlots()
{
    drawBorder();
    initSnake();
    moveSnake();
    createFood();

    QString viewText = QString::number (score,10);
    QString labelText = "<html><body>"
                        "<font size=\"4\">score: </font>" + viewText + "<br/>" +
                        "<font size=\"4\">level: </font>" + QString::number(level, 10) + "<br/><br/>" +
                        "<font size=\"4\">规则：</font><br/>" +
                        "<font size=\"2\">继续/上：箭头上<br/>" +
                        "继续/下：箭头下<br/>" +
                        "继续/左：箭头左<br/>" +
                        "继续/右：箭头右<br/>" +
                        "暂停/开始：空格<br/>"+
                        "shift:加速"
                        "</body></html>";
    mLabel->setText(labelText);

    timer.start(moveSpeed);
    //隐藏按钮，设其不可用
    startGame->setEnabled(false);
    startGame->hide();
}

//游戏结束
void Snake::gameOver()
{
    timer.stop();   // 计时器停止
    whether_stop = 1; //按键开始

    QMessageBox msgBox;
    msgBox.setWindowTitle("提醒:");
    msgBox.setText("游戏结束，复活 or 重新开始?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    QAbstractButton* okButton = msgBox.button(QMessageBox::Ok);
    QAbstractButton* cancelButton = msgBox.button(QMessageBox::Cancel);
    okButton->setText("复活");
    cancelButton->setText("重新开始");

    int ret = msgBox.exec();

    if(ret == QMessageBox::Ok){
        QKeyEvent *e;
        accelerate = 0;
        keyPressEvent(e);
    }
    else{
        score = 0;      // 初始化数据
        level = 1;
        foodCount = 0;
        moveSpeed = 400;
        accelerate = 0;
        for(int x = 1; x < MAX_X-1; x++)
        {
            for(int y = 1; y < MAX_Y-1; y++)
            {
                this->matrix[x][y]->type = NORMAL_LABEL;
                this->matrix[x][y]->label->hide();
            }
        }

        drawBorder();
        // 显示按钮，可以进行下一次游戏
        startGame->setEnabled(true);
        startGame->show();
    }
}

//游戏胜利
void Snake::victory()
{

}

// 键盘事件
void Snake::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    // 同方向或者反方向不做任何操作
    case Qt::Key_Up:
        if(up())
        {
            directionY = -1;
            directionX = 0;
            moveSnake();
            if(whether_stop == 1){
                whether_stop = 0;
                timer.start(moveSpeed);
            }
        }
        break;
    case Qt::Key_Down:
        if(down())
        {
            directionY = 1;
            directionX = 0;
            moveSnake();
            if(whether_stop == 1){
                whether_stop = 0;
                timer.start();
            }
        }
        break;
    case Qt::Key_Left:
        if(left())
        {
            directionY = 0;
            directionX = -1;
            moveSnake();
            if(whether_stop == 1){
                whether_stop = 0;
                timer.start();
            }

        }
        break;
    case Qt::Key_Right:
        if(right())
        {
            directionY = 0;
            directionX = 1;
            moveSnake();
            if(whether_stop == 1){
                whether_stop = 0;
                timer.start();
            }
        }
        break;
    // 游戏暂停
    case Qt::Key_Space:
        if(timer.isActive())
        {
            timer.stop();
            whether_stop = 1;
        }
        else
        {
            timer.start();
            whether_stop = 0;
        }
        break;
    //加速
    case Qt::Key_Shift:
        if(accelerate == 0){
            qDebug()<<"开始加速";
            accelerate = 1;
        }
        else{
            accelerate = 0;
            qDebug()<<"停止加速";
        }
        break;
    default:
        break;
    }
}

// 生成食物
void Snake::createFood()
{
    int foodX;
    int foodY;
    // 随机生成食物坐标
    srand((unsigned)time(0));
    do
    {
        foodX = rand()%MAX_X;
        foodY = rand()%MAX_Y;
    }while(matrix[foodX][foodY]->type != NORMAL_LABEL);

    matrix[foodX][foodY]->type = FOOD_LABEL;
    matrix[foodX][foodY]->label->setStyleSheet("background:lightblue");
    matrix[foodX][foodY]->label->show();
}

// 移动判断
bool Snake::up()
{
    if(directionX == 0)
    {
        return false;
    }
    return true;
}

bool Snake::down()
{
    if(directionX == 0)
    {
        return false;
    }
    return true;
}

bool Snake::left()
{
    if(directionY == 0)
    {
        return false;
    }
    return true;
}

bool Snake::right()
{
    if(directionY == 0)
    {
        return false;
    }
    return true;
}

