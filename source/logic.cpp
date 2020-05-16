#include "logic.h"


Logic::Logic(QObject *parent) : QObject(parent){
    connect(&b, SIGNAL(moved(int *const *const, int, bool)), w.board, SLOT(setBoard(int *const *const, int, bool)));
    connect(w.board, SIGNAL(sendMove(const QPoint&, const QPoint&)), &b,
            SLOT(move(const QPoint&, const QPoint&)));
    connect(&b, SIGNAL(promotion(bool)), &w, SIGNAL(promotion(bool)));
    connect(&w, SIGNAL(promotion(int)), &b, SLOT(promotion(int)));
    connect(&w, SIGNAL(newGame()), &b, SLOT(initBoard()));
    b.initBoard();
}
