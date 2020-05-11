#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QPoint>
#include <tuple>
#include "men.h"
#include "status.h"


class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(bool turn = true, QObject *parent = nullptr);
private:
    bool turn;
    Men **fullBoard; // tu check checks he-he 12*12
    Men **board; // just a part of fullBoard 8*8


signals:
    void moved(int *const *const board, int status);
    void promotion();
    void message(QString msg);
public slots:
    void move(const QPoint& from, const QPoint &to);
//    void promotion(int men);
    void initBoard();
private:
    int N = 8;
    
    QPoint WhiteKing;
    QPoint BlackKing;
    std::tuple<Men, QPoint, QPoint, Men> lastMove{Men::None, {0,0}, {0,0}, Men::None};
    std::tuple<Men, QPoint, QPoint, Men> tempMove{Men::None, {0,0}, {0,0}, Men::None};
//    bool canMove(const QPoint &from, const QPoint &to);
//    bool canMovePawn(const QPoint &from, const QPoint &to);
//    bool canMoveKnight(const QPoint &from, const QPoint &to);
//    bool canMoveRook(const QPoint &from, const QPoint &to);
//    bool canMoveBishop(const QPoint &from, const QPoint &to);
//    bool canMoveQueen(const QPoint &from, const QPoint &to);
//    bool canMoveKing(const QPoint &from, const QPoint &to);
//    bool canMove(const QPoint &from);
    
    Status status;
    bool hasCheck(bool isWhite);
    bool nChecks(bool isWhite);
//    bool hasMate(bool isWhite = true);
//    bool hasStaleMate(bool isWhite = true);
//    bool hasDraw(bool isWhite = true);
    
    
};

#endif // BOARD_H
