#ifndef SNAKECOLOR_H
#define SNAKECOLOR_H

#include "snake.h"
#include <QObject>

class Snakecolor : public Snake {
    Q_OBJECT
public:
    explicit Snakecolor(QWidget *parent = nullptr);
    ~Snakecolor();

    QString changeColor();

private:
    QString snakeColor;
    static int num;
    QString  color_arr[5] = {"background:red", "background:blue", "background:black", "background:white", "background:green"};
};

#endif // SNAKECOLOR_H
