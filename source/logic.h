#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include "widget.h"
#include "board.h"

class Logic : public QObject
{
    Q_OBJECT

    Board b;
    Widget w;
public:
    explicit Logic(QObject *parent = nullptr);
    void show(){w.show();}
signals:

public slots:
};

#endif // LOGIC_H
