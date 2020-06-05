#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include "mainwindow.h"
#include "board.h"
#include "board_replay.h"
#include "db_manager.h"

class Logic : public QObject
{
    Q_OBJECT

    Board bg;
    BoardReplay br;
    MainWindow mw;
    DBManager db;
public:
    explicit Logic(QObject *parent = nullptr);
    void show(){mw.show();}
signals:

public slots:
};

#endif // LOGIC_H
