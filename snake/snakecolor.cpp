#include "snakecolor.h"
#include <QKeyEvent>

int Snakecolor::num = 0;
Snakecolor::Snakecolor(QWidget *parent) : Snake(parent)
{

}

Snakecolor::~Snakecolor()
{
}

QString Snakecolor::changeColor()
{
    snakeColor = color_arr[num];
    num++;
    if(num == 4){
        num = 0;
    }
    return snakeColor;
}
