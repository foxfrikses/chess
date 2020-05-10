#include "logic.h"


Logic::Logic(QObject *parent) : QObject(parent){
    connect(&b, SIGNAL(moved(int *const *const)), w.board, SLOT(setBoard(int *const *const)));
    connect(w.board, SIGNAL(sendMove(const QPoint&, const QPoint&)), &b,
            SLOT(move(const QPoint&, const QPoint&)));
}
