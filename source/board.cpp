#include "board.h"

Board::Board(bool t, QObject *parent) : QObject(parent), turn{t}{
   fullBoard = new Men*[N + 4];
   for (int i = 0; i < N + 4; ++i){
       fullBoard[i] = new Men[N + 4];
   }
   board = new Men*[N];
   for (int i = 0; i < N; ++i) {
       board[i] = &fullBoard[i + 2][2];
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

    lastMove = {board[from.x()][from.y()], from, to, board[to.x()][to.y()]};
    board[to.x()][to.y()] = board[from.x()][from.y()];
    board[from.x()][from.y()] = Men::None;
    turn = !turn;
    emit moved((int *const*const)board, int(status));
}

void Board::initBoard(){
    turn = true;
    status = Status::Play;
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

bool Board::nChecks(bool isWhite)
{
    int result = 0;
    int x = (isWhite)? WhiteKing.x() : BlackKing.x();
    int y = (isWhite)? WhiteKing.y() : BlackKing.y();
    Men pawn = (isWhite)? Men::BPawn : Men::WPawn;
    Men knight = (isWhite)? Men::BKnight : Men::WKnight;
    Men queen = (isWhite)? Men::BQueen : Men::WQueen;
    Men rook = (isWhite)? Men::BRook : Men::WRook;
    Men bishop = (isWhite)? Men::BBishop : Men::WBishop;

    // check King
    if (abs(WhiteKing.x() - BlackKing.x()) <= 1 && abs(WhiteKing.y() - BlackKing.y()) <= 1)
        return true;

    // check Pawn
    if (board[x + 1][y + int(pawn)] == pawn || board[x - 1][y + int(pawn)] == pawn)
        return true;

    // check Knight
    if(board[x + 1][y + 2] == knight) ++result;
    if(board[x + 1][y - 2] == knight) ++result;
    if(board[x - 1][y + 2] == knight) ++result;
    if(board[x - 1][y - 2] == knight) ++result;
    if(board[x + 2][y + 1] == knight) ++result;
    if(board[x + 2][y - 1] == knight) ++result;
    if(board[x - 2][y + 1] == knight) ++result;
    if(board[x - 2][y + 1] == knight) ++result;

    // gorizontal check Queen or Rook
    for (int ix = x + 1; ix < N; ++ix){
        if (board[ix][y] == Men::None) continue;
        if (board[ix][y] == rook || board[ix][y] == queen) ++result;
        break;
    }
    for (int ix = x - 1; ix >= 0; --ix){
        if (board[ix][y] == Men::None) continue;
        if (board[ix][y] == rook || board[ix][y] == queen) ++result;
        break;
    }
    for (int iy = y + 1; iy < N; ++iy){
        if (board[x][iy] == Men::None) continue;
        if (board[x][iy] == rook || board[x][iy] == queen) ++result;
        break;
    }
    for (int iy = y - 1; iy >= 0; --iy){
        if (board[x][iy] == Men::None) continue;
        if (board[x][iy] == rook || board[x][iy] == queen) ++result;
        break;
    }

    // dioganal check Queen or Bishop
    for (int ix = x + 1, iy = y + 1; ix < N && iy < N; ++ix, ++iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) ++result;
        break;
    }
    for (int ix = x + 1, iy = y - 1; ix < N && iy >= 0; ++ix, --iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) ++result;
        break;
    }
    for (int ix = x - 1, iy = y + 1; ix >= 0 && iy < N; --ix, ++iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) ++result;
        break;
    }
    for (int ix = x - 1, iy = y - 1; ix >= 0 && iy >= 0; --ix, --iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) ++result;
        break;
    }

    return result;
}

bool Board::hasCheck(bool isWhite)
{
    int n = nChecks(isWhite);
    if (n == 0) status = Status::Play;
    else if (n == 1) status = Status::Check;
    else if (n > 1) status = Status::DoubleCheck;
    return n > 0;
}
