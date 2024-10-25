#ifndef MAINSENCE_H
#define MAINSENCE_H
#include <QMainWindow>
#include <QLabel>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <ctime>
#include <QPushButton>
#include <QFont>
#include <QSpinBox>
#include<mainsence.h>
#include"mypushbutton.h"
class mainsence : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainsence(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);


signals:

public slots:
};

#endif // MAINSENCE_H
