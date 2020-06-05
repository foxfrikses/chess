#ifndef BOARDPLAYGAME_H
#define BOARDPLAYGAME_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <tuple>
#include "men.h"
#include "status.h"

class BoardReplay : public QObject
{
    Q_OBJECT
public:
    explicit BoardReplay(QObject *parent = nullptr);

    struct Move{
        int32_t cell_from;
        int32_t cell_to;
        int32_t cell_add1;
        int32_t cell_add2;
        int32_t status;
    };
private:
    QVector<Move> moves;
    Status status;
    bool turn;
    int **board;
    int nMove;
public slots:
    void initBoard(QVector<Move> moves);
    void move(int nMove);
signals:
    void moved(int*const*const board, int status, bool turn);
};

#endif // BOARDPLAYGAME_H
