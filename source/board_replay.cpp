#include "board_replay.h"

BoardReplay::BoardReplay(QObject *parent) : QObject(parent)
{
    board = new int*[8];
    for (int i = 0; i < 8; ++i){
        board[i] = new int[8];
    }
}

void BoardReplay::initBoard(QVector<Move> moves)
{
    this->moves = QVector<Move>(moves);
    turn = true;
    status = Status::Play;
    nMove = -1;
    for (int i = 0; i < 8; ++i){
            board[i][2] = board[i][3] = board[i][4] = board[i][5] = Men::None;
            board[i][6] = Men::WPawn;
            board[i][1] = Men::BPawn;
    }
    board[1][0] = board[6][0] = Men::BKnight;
    board[1][7] = board[6][7] = Men::WKnight;
    board[2][0] = board[5][0] = Men::BBishop;
    board[2][7] = board[5][7] = Men::WBishop;
    board[0][0] = board[7][0] = Men::BRook;
    board[0][7] = board[7][7] = Men::WRook;
    board[3][0] = Men::BQueen;
    board[3][7] = Men::WQueen;
    board[4][0] = Men::BKing;
    board[4][7] = Men::WKing;
    emit moved(board, int(status), turn);
}

void BoardReplay::move(int nMove)
{
    if (nMove != this->nMove && nMove >= 0 && nMove < moves.count()){
        int men, x, y;
        while (nMove > this->nMove){
            ++this->nMove;
            int ms[4] = {moves[this->nMove].cell_from, moves[this->nMove].cell_to,
                         moves[this->nMove].cell_add1, moves[this->nMove].cell_add2};
            for (int i = 0; i < 4; ++i) {
                if (ms[i] == 0) continue;
                x = ms[i] % 13; ms[i] /= 13;
                y = ms[i] % 13; ms[i] /= 13;
                men = (ms[i] / 13) % 13 - 6;
                board[x][y] = men;
            }
        }
        while (nMove < this->nMove){
            int ms[4] = {moves[this->nMove].cell_from, moves[this->nMove].cell_to,
                         moves[this->nMove].cell_add1, moves[this->nMove].cell_add2};
            for (int i = 0; i < 4; ++i) {
                if (ms[i] == 0) continue;
                x = ms[i] % 13; ms[i] /= 13;
                y = ms[i] % 13; ms[i] /= 13;
                men = ms[i] % 13 - 6;
                board[x][y] = men;
            }
            --this->nMove;
        }
        status = Status(moves[this->nMove].status);
        turn = nMove % 2;
        emit moved(board, int(status), turn);
    }
}
