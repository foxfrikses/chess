#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QPoint>
#include <tuple>
#include "men.h"

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(bool turn = true, QObject *parent = nullptr);
private:
    bool turn;
    std::tuple<Men, QPoint, QPoint> lastMove{Men::None, {0,0}, {0,0}};
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
