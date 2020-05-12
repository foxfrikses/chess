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
    if (board[from.x()][from.y()] == Men::None)
        return;
    if ((board[from.x()][from.y()] > Men::None && !turn)
            || (board[from.x()][from.y()] < Men::None && turn))
        return;

    if(canMove(from, to)){
        board[to.x()][to.y()] = board[from.x()][from.y()];
        board[from.x()][from.y()] = Men::None;
        turn = !turn;
        emit moved((int*const*const)board, int(status));
    }
}

void Board::initBoard(){
    turn = true;
    status = Status::Play;
    for (int i = 0; i < N; ++i){
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
    board[4][0] = Men::BKing; BlackKing = {4, 0};
    board[4][7] = Men::WKing; WhiteKing = {4, 7};
    
}

bool Board::canMove(const QPoint &from, const QPoint &to){
    if (status == Status::Mate || status == Status::Draw || status == Status::StaleMate)
        return false;

    Men pawn    = (turn)? Men::WPawn    : Men::BPawn;
    Men knight  = (turn)? Men::WKnight  : Men::BKnight;
    Men queen   = (turn)? Men::WQueen   : Men::BQueen;
    Men rook    = (turn)? Men::WRook    : Men::BRook;
    Men bishop  = (turn)? Men::WBishop  : Men::BBishop;
    Men king    = (turn)? Men::WKing    : Men::BKing;

    if (pawn * board[to.x()][to.y()] > 0) return false;
    if (pawn * board[from.x()][from.y()] <= 0) return false;
    if (to == from) return false;

    if (board[from.x()][from.y()] == king) { // KING
        (turn)? WhiteKing = to : BlackKing = to;
        return true; // I will change it =)
    }


    QPoint dPoint = from - to;
    QVector<Point> points;
    points.append({from, board[from.x()][from.y()]});
    points.append({to,   board[to.x()][to.y()]});

    if (board[from.x()][from.y()] == pawn){ // PAWN
        if (dPoint.x() == 0){ // just move
            if (dPoint.y() == int(pawn)){ // move 1 cell
                if (board[to.x()][to.y()] != Men::None)
                    return false;
            }else if (dPoint.y() == 2 * int(pawn)){ // move 2 cells
                if (not (from.y() == ((turn)?6:1) && board[to.x()][to.y()] == Men::None
                                           && board[to.x()][to.y() + int(pawn)] == Men::None))
                    return false;
            } else return false;
        }else if (dPoint.x() == 1 || dPoint.x() == -1) { // take men
            if (dPoint.y() == int(pawn)){
                if (int(board[to.x()][to.y()]) * int(pawn) < 0){ // check diff colors
                    return false;
                } else if (board[to.x()][to.y()] == Men::None){ // взятие на проходе
                    if (lastMove.moved == -pawn && lastMove.beated == Men::None &&
                            lastMove.from == QPoint{to.x(), to.y() + int(pawn)}){
                        points.append(Point{QPoint{to.x(), from.y()},Men(-pawn)});
                        board[to.x()][from.y()] = Men::None;
                    }
                    else return false;
                }
            } else return false;
        } else return false;

    } else if (board[from.x()][from.y()] == knight){ // KNIGHT
        if ((abs(dPoint.x()) != 1 || abs(dPoint.y()) != 2)
                && (abs(dPoint.x()) != 2 || abs(dPoint.y()) != 1))
            return false;

    } else if (board[from.x()][from.y()] == bishop) { // BISHOP
        if (abs(dPoint.x()) != abs(dPoint.y())) return false;
        if       (dPoint.x() > 0 && dPoint.y() > 0){
            for (int ix = from.x() - 1, iy = from.y() - 1; ix > to.x(); --ix, --iy)
                if (board[ix][iy] != Men::None) return false;
        }else if (dPoint.x() < 0 && dPoint.y() < 0){
            for (int ix = from.x() + 1, iy = from.y() + 1; ix < to.x(); ++ix, ++iy)
                if (board[ix][iy] != Men::None) return false;
        }else if (dPoint.x() < 0 && dPoint.y() > 0){
            for (int ix = from.x() + 1, iy = from.y() - 1; ix < to.x(); ++ix, --iy)
                if (board[ix][iy] != Men::None) return false;
        }else{  //if (dPoint.x() < 0 && dPoint.y() < 0){
            for (int ix = from.x() - 1, iy = from.y() + 1; ix > to.x(); --ix, ++iy)
                if (board[ix][iy] != Men::None) return false;
        }

    } else if (board[from.x()][from.y()] == rook) { // ROOK
        if (dPoint.x() != 0 && dPoint.y() != 0) return false;
        if       (dPoint.x() > 0){
            for (int ix = from.x() - 1; ix > to.x(); --ix)
                if (board[ix][from.y()] != Men::None) return false;
        }else if (dPoint.x() < 0){
            for (int ix = from.x() + 1; ix < to.x(); ++ix)
                if (board[ix][from.y()] != Men::None) return false;
        }else if (dPoint.y() > 0){
            for (int iy = from.y() - 1; iy > to.y(); --iy)
                if (board[from.x()][iy] != Men::None) return false;
        }else  if (dPoint.y() < 0){
            for (int iy = from.y() + 1; iy < to.y(); ++iy)
                if (board[from.x()][iy] != Men::None) return false;
        }

    } else if (board[from.x()][from.y()] == queen) { // QUEEN
        if (abs(dPoint.x()) == abs(dPoint.y())){
            if       (dPoint.x() > 0 && dPoint.y() > 0){
                for (int ix = from.x() - 1, iy = from.y() - 1; ix > to.x(); --ix, --iy)
                    if (board[ix][iy] != Men::None) return false;
            }else if (dPoint.x() < 0 && dPoint.y() < 0){
                for (int ix = from.x() + 1, iy = from.y() + 1; ix < to.x(); ++ix, ++iy)
                    if (board[ix][iy] != Men::None) return false;
            }else if (dPoint.x() < 0 && dPoint.y() > 0){
                for (int ix = from.x() + 1, iy = from.y() - 1; ix < to.x(); ++ix, --iy)
                    if (board[ix][iy] != Men::None) return false;
            }else if (dPoint.x() < 0 && dPoint.y() < 0){
                for (int ix = from.x() - 1, iy = from.y() + 1; ix > to.x(); --ix, ++iy)
                    if (board[ix][iy] != Men::None) return false;
            }
        }else if (dPoint.x() == 0 || dPoint.y() == 0){
            if       (dPoint.x() > 0){
                for (int ix = from.x() - 1; ix > to.x(); --ix)
                    if (board[ix][from.y()] != Men::None) return false;
            }else if (dPoint.x() < 0){
                for (int ix = from.x() + 1; ix < to.x(); ++ix)
                    if (board[ix][from.y()] != Men::None) return false;
            }else if (dPoint.y() > 0){
                for (int iy = from.y() - 1; iy > to.y(); --iy)
                    if (board[from.x()][iy] != Men::None) return false;
            }else  if (dPoint.y() < 0){
                for (int iy = from.y() + 1; iy < to.y(); ++iy)
                    if (board[from.x()][iy] != Men::None) return false;
            }
        } else return false;
    }

    board[to.x()][to.y()] = board[from.x()][from.y()];
    board[from.x()][from.y()] = Men::None;
    bool result = !hasCheck(turn);
    for (auto p: points)
        board[p.point.x()][p.point.y()] = p.man;
    return result;
}

int Board::nChecks(bool isWhite)
{
    int result = 0;
    int x = (isWhite)? WhiteKing.x() : BlackKing.x();
    int y = (isWhite)? WhiteKing.y() : BlackKing.y();
    Men pawn    = (isWhite)? Men::BPawn : Men::WPawn;
    Men knight  = (isWhite)? Men::BKnight : Men::WKnight;
    Men queen   = (isWhite)? Men::BQueen : Men::WQueen;
    Men rook    = (isWhite)? Men::BRook : Men::WRook;
    Men bishop  = (isWhite)? Men::BBishop : Men::WBishop;

    // check King
    if (abs(WhiteKing.x() - BlackKing.x()) <= 1 && abs(WhiteKing.y() - BlackKing.y()) <= 1)
        ++result;

    // check Pawn
    if (board[x + 1][y + int(pawn)] == pawn) ++result;
    if (board[x - 1][y + int(pawn)] == pawn) ++result;

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

bool Board::hasCheck(bool isWhite){
    int n = nChecks(isWhite);
    if (n == 0) status = Status::Play;
    else if (n == 1) status = Status::Check;
    else if (n > 1) status = Status::DoubleCheck;
    return n > 0;
}
