#include "snake.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPainter>
#include <QTimer>
#include <windows.h>
#include"mypushbutton.h"
#include"mainsence.h"
#include <QtMultimedia/QMediaPlayer>
#include <QSound>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建启动画面对象
    QSplashScreen splash;
    splash.setPixmap(QPixmap(":/rsc/images/njustlable.jpg"));
    splash.show();

    QFont font("Microsoft YaHei", 12);  // 设置字体为微软雅黑，大小为12
    splash.setFont(font);
    // 在启动画面上绘制文字
    splash.showMessage("加载中...", Qt::AlignBottom | Qt::AlignCenter, Qt::black);

    // 延迟一段时间关闭启动画面，3秒
    QTimer::singleShot(2000, &splash, &QSplashScreen::close);
    Sleep(2000);//显示启动画面
    mainsence *s=new mainsence;
    s->show();

    return a.exec();
}
