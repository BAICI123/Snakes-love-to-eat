#include "mypushbutton.h"
#include<QDebug>
#include<QPropertyAnimation>
#include "snake.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPainter>
#include <QTimer>
#include <windows.h>
#include"mainsence.h"

mainsence::mainsence(QWidget *parent) : QMainWindow(parent)
{

    //    设置主界面格式和背景
    setFixedSize(1260,900);
    setWindowIcon(QIcon(":/rsc/images/njustlable"));
    setWindowTitle("方块贪吃蛇主界面");

    Mypushbutton * startbtn=new Mypushbutton(":/rsc/images/1llb9zif.png");
    startbtn->setParent(this);
    startbtn->move(this->width()*0.5-startbtn->width()*0.5,this->height()*0.55);

    connect(startbtn,&Mypushbutton::clicked,[=](){

        startbtn->zoom1();

        this->hide();
        Snake   *w=new Snake;
        w->show();
    });

}

void mainsence::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/rsc/images/snakebackground.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}





