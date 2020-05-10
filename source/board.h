#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QPoint>
#include "men.h"

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(bool turn = true, QObject *parent = nullptr);
private:
    bool turn;
    Men **board;


signals:
    void moved(int *const *const board);
public slots:
    void move(const QPoint& from, const QPoint &to);
    void initBoard();
private:

    int N = 8;
};

#endif // BOARD_H
