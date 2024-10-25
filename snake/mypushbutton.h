#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include"snake.h"

class Mypushbutton : public QPushButton
{
    Q_OBJECT
public:
   // explicit Mypushbutton(QWidget *parent = nullptr);
    Mypushbutton(QString normalimg,QString pressimg="");
    QString normalpath;
    QString presspath;
    void zoom1();

signals:

public slots:
};

#endif // MYPUSHBUTTON_H
