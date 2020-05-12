#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <tuple>
#include "men.h"
#include "status.h"

struct MoveStruct{Men moved; Men beated; QPoint from; QPoint to; QPoint beatedPoint;};
struct Point{QPoint point; Men man;};

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

    bool canMove(const QPoint &from, const QPoint &to);
//    bool canMovePawn(const QPoint &from, const QPoint &to);
//    bool canMoveKnight(const QPoint &from, const QPoint &to);
//    bool canMoveRook(const QPoint &from, const QPoint &to);
//    bool canMoveBishop(const QPoint &from, const QPoint &to);
//    bool canMoveQueen(const QPoint &from, const QPoint &to);
//    bool canMoveKing(const QPoint &from, const QPoint &to);
//    bool canMove(const QPoint &from);
    
    MoveStruct lastMove{Men::None, Men::None, {-1,-1}, {-1,-1}, {-1,-1}};
    Status status;
    bool hasCheck(bool isWhite);
    int nChecks(bool isWhite);
//    bool hasMate(bool isWhite = true);
//    bool hasStaleMate(bool isWhite = true);
//    bool hasDraw(bool isWhite = true);
    
    
};

#endif // BOARD_H
