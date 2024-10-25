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
Mypushbutton::Mypushbutton(QString normalimg,QString pressimg){
    this->normalpath=normalimg;
    this->presspath=pressimg;

    QPixmap pix;
    pix.load(normalimg);
    //设置图片大小
    this->setFixedSize(pix.width(),pix.height());
    //设置图标
    this->setStyleSheet("QPushButton{border:0px;");
    this->setIcon(pix);
    //设置图标大小
    this->setIconSize(QSize(pix.width(),pix.height()));


}
void Mypushbutton:: zoom1(){
    //创建对象
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry");
    //设置动画时间间隔
    animation->setDuration(200);
    //起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //结束位置
      animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    //设置弹跳曲线
      animation->setEasingCurve(QEasingCurve::OutBounce);
      //动画执行
      animation->start();

}
