#include "board.h"

Board::Board(bool t, QObject *parent) : QObject(parent), turn{t}{
   fullBoard = new Men*[N + 4];
   for (int i = 0; i < N + 4; ++i){
       fullBoard[i] = new Men[N + 4];
   }
   board = new Men*[N + 4];
   for (int i = 0; i < N + 4; ++i) {
       board[i] = &fullBoard[i][2];
   }
   ++(++board);
   initBoard();
}

void Board::move(const QPoint& from, const QPoint &to){
    if (board[from.x()][from.y()] == Men::None)
        return;
    if ((board[from.x()][from.y()] > Men::None && !turn)
            || (board[from.x()][from.y()] < Men::None && turn))
        return;

    if(canMove(from, to)){
        lastMove = {board[from.x()][from.y()], board[to.x()][to.y()], from, to, to};
        if (abs(board[from.x()][from.y()]) == Men::WPawn){
            if(abs(from.x() - to.x()) == 1 && board[to.x()][to.y()] == Men::None){
                lastMove.beated = board[to.x()][from.y()];
                lastMove.beatedPoint = {to.x(),from.y()};
                board[to.x()][from.y()] = Men::None;
            }
            if (    (board[from.x()][from.y()] > 0 && to.y() == 0) ||
                    (board[from.x()][from.y()] < 0 && to.y() == 7) ){
                board[to.x()][to.y()] = board[from.x()][from.y()];
                board[from.x()][from.y()] = Men::None;
                emit promotion(turn);
                return;
            }

        } else if (abs(board[from.x()][from.y()]) == Men::WKing){
            if(abs(from.x() - to.x()) > 1){
                if(to.x() == 6){
                    board[5][to.y()] = board[7][to.y()];
                    board[7][to.y()] = Men::None;
                }else if(to.x() == 2){
                    board[3][to.y()] = board[0][to.y()];
                    board[0][to.y()] = Men::None;
                }else throw "Всё хуйня, давай по новой";
            }
            if (board[from.x()][from.y()] == Men::WKing){
                WhiteKing = to;
                WKMoved = true;
            }
            else{
                BlackKing = to;
                BKMoved = true;
            }
        }


        board[to.x()][to.y()] = board[from.x()][from.y()];
        board[from.x()][from.y()] = Men::None;
        turn = !turn;
        emit moved((int*const*const)board, int(status));

    }
}

void Board::promotion(int man)
{
    if (lastMove.moved * man <= 0 || abs(man) < 2 || abs(man) > 5) emit promotion(turn);
    else{
        board[lastMove.to.x()][lastMove.to.y()] = Men(man);
        turn = !turn;
        emit moved((int*const*const)board, int(status));
    }
}

void Board::initBoard(){
    turn = true;
    status = Status::Play;
    WKMoved = false; BKMoved = false;
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
    QPoint dPoint = from - to;
    QVector<Point> points;
    points.append({from, board[from.x()][from.y()]});
    points.append({to,   board[to.x()][to.y()]});

    if (board[from.x()][from.y()] == king) { // KING
        if (abs(dPoint.x()) > 1 || abs(dPoint.y()) > 1){
            if((turn & WKMoved)|(!turn & BKMoved) || status != Status::Play) return false;
            else if (to.x() == 6 && to.y() == from.y()) {
                if (    board[7][to.y()] != rook ||
                        board[6][to.y()] != Men::None ||
                        board[5][to.y()] != Men::None) return false;
                points.append({{7, to.y()},   rook});
                points.append({{5, to.y()},   Men::None});
                bool result = hasCheck(turn);
                board[5][to.y()] = king;
                board[4][to.y()] = Men::None;
                if (king > 0) WhiteKing = {5, to.y()};
                else BlackKing = {5, to.y()};
                result |= hasCheck(turn);
                if (king > 0) WhiteKing = from;
                else BlackKing = from;
                board[4][to.y()] = king;
                board[5][to.y()] = Men::None;
                if (result) return false;
                board[5][to.y()] = rook;
                board[7][to.y()] = Men::None;
            }else if (to.x() == 2 && to.y() == from.y()) {
                if (    board[0][to.y()] != rook ||
                        board[1][to.y()] != Men::None ||
                        board[2][to.y()] != Men::None ||
                        board[3][to.y()] != Men::None) return false;
                points.append({{0, to.y()},   rook});
                points.append({{1, to.y()},   Men::None});
                points.append({{3, to.y()},   Men::None});
                bool result = hasCheck(turn);
                board[3][to.y()] = king;
                board[4][to.y()] = Men::None;
                if (king > 0) WhiteKing = {3, to.y()};
                else BlackKing = {3, to.y()};
                result |= hasCheck(turn);
                if (king > 0) WhiteKing = from;
                else BlackKing = from;
                board[4][to.y()] = king;
                board[3][to.y()] = Men::None;
                if (result) return false;
                board[3][to.y()] = rook;
                board[0][to.y()] = Men::None;
            }else return false;
        }
        board[to.x()][to.y()] = board[from.x()][from.y()];
        board[from.x()][from.y()] = Men::None;
        if (king > 0) WhiteKing = to;
        else BlackKing = to;
        bool result = !hasCheck(turn);
        for (auto p: points)
            board[p.point.x()][p.point.y()] = p.man;
        if (king > 0) WhiteKing = from;
        else BlackKing = from;
        return result;
    }

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
        }else if (abs(dPoint.x()) == 1) { // take men
            if (dPoint.y() == int(pawn)){
                if (int(board[to.x()][to.y()]) * int(pawn) > 0){ // check diff colors
                    return false;
                } else if (board[to.x()][to.y()] == Men::None){ // взятие на проходе
                    if (lastMove.moved == -pawn && lastMove.beated == Men::None &&
                            lastMove.from == QPoint{to.x(), to.y() - int(pawn)}){
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
    result = result + 1 - 1;

    // check Knight
    if(board[x + 1][y + 2] == knight) ++result;
    if(board[x + 1][y - 2] == knight) ++result;
    if(board[x - 1][y + 2] == knight) ++result;
    if(board[x - 1][y - 2] == knight) ++result;
    if(board[x + 2][y + 1] == knight) ++result;
    if(board[x + 2][y - 1] == knight) ++result;
    if(board[x - 2][y + 1] == knight) ++result;
    if(board[x - 2][y - 1] == knight) ++result;

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
