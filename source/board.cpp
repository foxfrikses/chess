#include "board.h"

Board::Board(bool t, QObject *parent) : QObject(parent), turn{t}{
   board = new Men*[N];
   for (int i = 0; i < N; ++i){
       board[i] = new Men[N];
   }
   initBoard();
}

void Board::move(const QPoint& from, const QPoint &to){
    if (from == to)
        return;
    if (board[from.x()][from.y()] == Men::None)
        return;
    if ((board[from.x()][from.y()] > Men::None && !turn)
            || (board[from.x()][from.y()] < Men::None && turn))
        return;

    board[to.x()][to.y()] = board[from.x()][from.y()];
    board[from.x()][from.y()] = Men::None;
    turn = !turn;
    lastMove = {board[to.x()][to.y()], from, to};
    emit moved((int *const*const)board);
}

void Board::initBoard(){
    turn = true;
    for (int i = 0; i < N; ++i){
            board[2][i] = board[3][i] = board[4][i] = board[5][i] = Men::None;
            board[6][i] = Men::WPawn;
            board[1][i] = Men::BPawn;
    }
    board[0][1] = board[0][6] = Men::BKnight;
    board[7][1] = board[7][6] = Men::WKnight;
    board[0][2] = board[0][5] = Men::BBishop;
    board[7][2] = board[7][5] = Men::WBishop;
    board[0][0] = board[0][7] = Men::BRook;
    board[7][0] = board[7][7] = Men::WRook;
    board[0][3] = Men::BQueen;
    board[7][3] = Men::WQueen;
    board[0][4] = Men::BKing;
    board[7][4] = Men::WKing;

}
