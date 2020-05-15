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

Board::~Board()
{
    for (int i = 0; i < N + 4; ++i){
        delete []fullBoard[i];
    }
    delete []fullBoard;
}

void Board::move(const QPoint& from, const QPoint &to){
    if (board[from.x()][from.y()] == Men::None)
        return;
    if ((board[from.x()][from.y()] > Men::None && !turn)
            || (board[from.x()][from.y()] < Men::None && turn))
        return;
    if (status == Status::Draw || status == Status::Mate || status == Status::StaleMate ||
            status == Status::Promotion) return;

    if(canMoveInTurn(from, to)){
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
                status = Status::Promotion;
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
        checkStatus();
        turn = !turn;
        emit moved((int*const*const)board, int(status));

    }
}

void Board::promotion(int man)
{
    if (lastMove.moved * man <= 0 || abs(man) < 2 || abs(man) > 5) emit promotion(turn);
    else{
        board[lastMove.to.x()][lastMove.to.y()] = Men(man);
        checkStatus();
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

bool Board::canManMove(const QPoint &from, const QPoint &to){
    if (from.x() < 0 || from.x() > 7 || from.y() < 0 || from.y() > 7 ||
        to.x() < 0   || to.x() > 7   || to.y() < 0   || to.y() > 7   ||
        board[from.x()][from.y()] == Men::None ||
        board[from.x()][from.y()] * board[to.x()][to.y()] > 0) return false;

    if      (abs(board[from.x()][from.y()]) == 1)
        return canPawnMove(from, to);
    else if (abs(board[from.x()][from.y()]) == 2)
        return canKnightMove(from, to);
    else if (abs(board[from.x()][from.y()]) == 3)
        return canBishopMove(from, to);
    else if (abs(board[from.x()][from.y()]) == 4)
        return canRookMove(from, to);
    else if (abs(board[from.x()][from.y()]) == 5)
        return canQueenMove(from, to);
    else if (abs(board[from.x()][from.y()]) == 6)
        return canKingMove(from, to);
    else return false;
}

bool Board::canMoveInTurn(const QPoint &from, const QPoint &to){
    if (status == Status::Mate || status == Status::Draw || status == Status::StaleMate)
        return false;
    if ((turn ? 1 : -1) * board[from.x()][from.y()] <= 0) return false;
    return canManMove(from, to);
}

bool Board::canPawnMove(const QPoint &from, const QPoint &to){
    bool turn{board[from.x()][from.y()] > 0};
    Men pawn = board[from.x()][from.y()];
    QPoint dPoint = from - to;
    QVector<Point> points;
    points.append({from, board[from.x()][from.y()]});
    points.append({to,   board[to.x()][to.y()]});

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

    board[to.x()][to.y()] = board[from.x()][from.y()];
    board[from.x()][from.y()] = Men::None;
    bool result = !hasCheck(turn);
    for (auto p: points)
        board[p.point.x()][p.point.y()] = p.man;
    return result;
}

bool Board::canKnightMove(const QPoint &from, const QPoint &to){
    QPoint dPoint = from - to;
    if ((abs(dPoint.x()) != 1 || abs(dPoint.y()) != 2)
            && (abs(dPoint.x()) != 2 || abs(dPoint.y()) != 1))
        return false;
    Men temp = board[to.x()][to.y()];
    board[to.x()][to.y()] = board[from.x()][from.y()];
    board[from.x()][from.y()] = Men::None;
    bool result = !hasCheck(turn);
    board[from.x()][from.y()] = board[to.x()][to.y()];
    board[to.x()][to.y()] = temp;
    return result;
}

bool Board::canRookMove(const QPoint &from, const QPoint &to){
    QPoint dPoint = from - to;
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
    Men temp = board[to.x()][to.y()];
    board[to.x()][to.y()] = board[from.x()][from.y()];
    board[from.x()][from.y()] = Men::None;
    bool result = !hasCheck(turn);
    board[from.x()][from.y()] = board[to.x()][to.y()];
    board[to.x()][to.y()] = temp;
    return result;
}

bool Board::canBishopMove(const QPoint &from, const QPoint &to){
    QPoint dPoint = from - to;
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
    Men temp = board[to.x()][to.y()];
    board[to.x()][to.y()] = board[from.x()][from.y()];
    board[from.x()][from.y()] = Men::None;
    bool result = !hasCheck(turn);
    board[from.x()][from.y()] = board[to.x()][to.y()];
    board[to.x()][to.y()] = temp;
    return result;
}

bool Board::canQueenMove(const QPoint &from, const QPoint &to){
    return canRookMove(from, to) || canBishopMove(from, to);
}

bool Board::canKingMove(const QPoint &from, const QPoint &to){
    bool turn{board[from.x()][from.y()] > 0};
    Men rook = (turn)? Men::WRook : Men::BRook;
    QPoint dPoint = from - to;
    QVector<Point> points;
    points.append({from, board[from.x()][from.y()]});
    points.append({to,   board[to.x()][to.y()]});

    if (abs(dPoint.x()) > 1 || abs(dPoint.y()) > 1){
        if((turn & WKMoved)|(!turn & BKMoved)) return false;
        else if (to.x() == 6 && to.y() == from.y()) {
            if (    board[7][to.y()] != rook ||
                    board[6][to.y()] != Men::None ||
                    board[5][to.y()] != Men::None) return false;
            points.append({{7, to.y()},   rook});
            points.append({{5, to.y()},   Men::None});
            bool result = hasCheck(turn);
            board[5][to.y()] = board[4][to.y()];
            board[4][to.y()] = Men::None;
            if (turn) WhiteKing = {5, to.y()};
            else BlackKing = {5, to.y()};
            result |= hasCheck(turn);
            if (turn) WhiteKing = from;
            else BlackKing = from;
            board[4][to.y()] = board[5][to.y()];
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
            board[3][to.y()] = board[4][to.y()];
            board[4][to.y()] = Men::None;
            if (turn) WhiteKing = {3, to.y()};
            else BlackKing = {3, to.y()};
            result |= hasCheck(turn);
            if (turn) WhiteKing = from;
            else BlackKing = from;
            board[4][to.y()] = board[3][to.y()];
            board[3][to.y()] = Men::None;
            if (result) return false;
            board[3][to.y()] = rook;
            board[0][to.y()] = Men::None;
        }else return false;
    }
    board[to.x()][to.y()] = board[from.x()][from.y()];
    board[from.x()][from.y()] = Men::None;
    if (turn) WhiteKing = to;
    else BlackKing = to;
    bool result = !hasCheck(turn);
    for (auto p: points)
        board[p.point.x()][p.point.y()] = p.man;
    if (turn) WhiteKing = from;
    else BlackKing = from;
    return result;
}

//bool Board::canPawnMove(const QPoint &from)
//{

//}

//bool Board::canKnightMove(const QPoint &from)
//{

//}

//bool Board::canRookMove(const QPoint &from)
//{

//}

//bool Board::canBishopMove(const QPoint &from)
//{

//}

//bool Board::canQueenMove(const QPoint &from)
//{

//}

bool Board::canKingMove(const QPoint &from)
{
    QVector<QPoint> points;
    bool t{from.y() > 0}, b{from.y() < 7}, l{from.x() > 0}, r{from.x() < 7};
    if (t)   points.append({from.x(), from.y() - 1});
    if (b)   points.append({from.x(), from.y() + 1});
    if (l)   points.append({from.x() - 1, from.y()});
    if (r)   points.append({from.x() + 1, from.y()});
    if (t&r) points.append({from.x() + 1, from.y() - 1});
    if (t&l) points.append({from.x() - 1, from.y() - 1});
    if (b&r) points.append({from.x() + 1, from.y() + 1});
    if (b&l) points.append({from.x() - 1, from.y() + 1});
    for (auto p : points) {
        Men temp = board[p.x()][p.y()];
        board[p.x()][p.y()] = board[from.x()][from.y()];
        board[from.x()][from.y()] = Men::None;
        bool result = !hasCheck(board[p.x()][p.y()] > 0);
        board[from.x()][from.y()] = board[p.x()][p.y()];
        board[p.x()][p.y()] = temp;
        if (result) return true;
    }
    return false;
}

bool Board::canAchive(QPoint to, bool isWhite)
{
    QVector<QPoint> points;
    int x = to.x();
    int y = to.y();
    Men pawn    = (isWhite)? Men::WPawn : Men::BPawn;
    Men knight  = (isWhite)? Men::WKnight : Men::BKnight;
    Men queen   = (isWhite)? Men::WQueen : Men::BQueen;
    Men rook    = (isWhite)? Men::WRook : Men::BRook;
    Men bishop  = (isWhite)? Men::WBishop : Men::BBishop;

    // Pawn
    if (board[x + 1][y + int(pawn)] == pawn) points.append({x + 1, y + int(pawn)});
    if (board[x - 1][y + int(pawn)] == pawn) points.append({x - 1, y + int(pawn)});
    if (board[x][y + int(pawn)]     == pawn) points.append({x, y + int(pawn)});
    if (board[x][y + 2 *int(pawn)]  == pawn) points.append({x, y + 2*int(pawn)});
    // Knight
    if(board[x + 1][y + 2] == knight) points.append({x + 1, y + 2});
    if(board[x + 1][y - 2] == knight) points.append({x + 1, y - 2});
    if(board[x - 1][y + 2] == knight) points.append({x - 1, y + 2});
    if(board[x - 1][y - 2] == knight) points.append({x - 1, y - 2});
    if(board[x + 2][y + 1] == knight) points.append({x + 2, y + 1});
    if(board[x + 2][y - 1] == knight) points.append({x + 2, y - 1});
    if(board[x - 2][y + 1] == knight) points.append({x - 2, y + 1});
    if(board[x - 2][y - 1] == knight) points.append({x - 2, y - 1});
    // gorizontal Queen or Rook
    for (int ix = x + 1; ix < N; ++ix){
        if (board[ix][y] == Men::None) continue;
        if (board[ix][y] == rook || board[ix][y] == queen) points.append({ix, y});
        break;
    }
    for (int ix = x - 1; ix >= 0; --ix){
        if (board[ix][y] == Men::None) continue;
        if (board[ix][y] == rook || board[ix][y] == queen) points.append({ix, y});
        break;
    }
    for (int iy = y + 1; iy < N; ++iy){
        if (board[x][iy] == Men::None) continue;
        if (board[x][iy] == rook || board[x][iy] == queen) points.append({x, iy});
        break;
    }
    for (int iy = y - 1; iy >= 0; --iy){
        if (board[x][iy] == Men::None) continue;
        if (board[x][iy] == rook || board[x][iy] == queen) points.append({x, iy});
        break;
    }
    // dioganal Queen or Bishop
    for (int ix = x + 1, iy = y + 1; ix < N && iy < N; ++ix, ++iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) points.append({ix, iy});
        break;
    }
    for (int ix = x + 1, iy = y - 1; ix < N && iy >= 0; ++ix, --iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) points.append({ix, iy});
        break;
    }
    for (int ix = x - 1, iy = y + 1; ix >= 0 && iy < N; --ix, ++iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) points.append({ix, iy});
        break;
    }
    for (int ix = x - 1, iy = y - 1; ix >= 0 && iy >= 0; --ix, --iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) points.append({ix, iy});
        break;
    }
    Men temp{board[x][y]};
    bool result{false};
    for (auto p : points) result |= canManMove(p, to);
    board[x][y] = temp;
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
    if (board[x + 1][y + int(pawn)] == pawn){ ++result; checkingMen = {x + 1, y + int(pawn)};}
    if (board[x - 1][y + int(pawn)] == pawn){ ++result; checkingMen = {x - 1, y + int(pawn)};}

    // check Knight
    if(board[x + 1][y + 2] == knight){ ++result; checkingMen = {x + 1, y + 2};}
    if(board[x + 1][y - 2] == knight){ ++result; checkingMen = {x + 1, y - 2};}
    if(board[x - 1][y + 2] == knight){ ++result; checkingMen = {x - 1, y + 2};}
    if(board[x - 1][y - 2] == knight){ ++result; checkingMen = {x - 1, y - 2};}
    if(board[x + 2][y + 1] == knight){ ++result; checkingMen = {x + 2, y + 1};}
    if(board[x + 2][y - 1] == knight){ ++result; checkingMen = {x + 2, y - 1};}
    if(board[x - 2][y + 1] == knight){ ++result; checkingMen = {x - 2, y + 1};}
    if(board[x - 2][y - 1] == knight){ ++result; checkingMen = {x - 2, y - 1};}

    // gorizontal check Queen or Rook
    for (int ix = x + 1; ix < N; ++ix){
        if (board[ix][y] == Men::None) continue;
        if (board[ix][y] == rook || board[ix][y] == queen) { ++result; checkingMen = {ix, y};}
        break;
    }
    for (int ix = x - 1; ix >= 0; --ix){
        if (board[ix][y] == Men::None) continue;
        if (board[ix][y] == rook || board[ix][y] == queen) { ++result; checkingMen = {ix, y};}
        break;
    }
    for (int iy = y + 1; iy < N; ++iy){
        if (board[x][iy] == Men::None) continue;
        if (board[x][iy] == rook || board[x][iy] == queen) { ++result; checkingMen = {x, iy};}
        break;
    }
    for (int iy = y - 1; iy >= 0; --iy){
        if (board[x][iy] == Men::None) continue;
        if (board[x][iy] == rook || board[x][iy] == queen) { ++result; checkingMen = {x, iy};}
        break;
    }

    // dioganal check Queen or Bishop
    for (int ix = x + 1, iy = y + 1; ix < N && iy < N; ++ix, ++iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) { ++result; checkingMen = {ix, iy};}
        break;
    }
    for (int ix = x + 1, iy = y - 1; ix < N && iy >= 0; ++ix, --iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) { ++result; checkingMen = {ix, iy};}
        break;
    }
    for (int ix = x - 1, iy = y + 1; ix >= 0 && iy < N; --ix, ++iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) { ++result; checkingMen = {ix, iy};}
        break;
    }
    for (int ix = x - 1, iy = y - 1; ix >= 0 && iy >= 0; --ix, --iy){
        if (board[ix][iy] == Men::None) continue;
        if (board[ix][iy] == bishop || board[ix][iy] == queen) { ++result; checkingMen = {ix, iy};}
        break;
    }

    return result;
}

void Board::checkStatus()
{
    int n = nChecks(!turn);
    bool KMove = canKingMove((turn)? BlackKing: WhiteKing);

    if (n == 0){
        if (KMove) status = Status::Play;
        else{
            // can move for each men
        }
    }
    else if (n == 1){
        status = Status::Check;
        if (!KMove) {
            Men man = board[checkingMen.x()][checkingMen.y()];
            QPoint kingPoint = (man > 0)? BlackKing : WhiteKing;
            int kx{kingPoint.x()}, ky{kingPoint.y()}, mx{checkingMen.x()}, my{checkingMen.y()};
            QPoint d = kingPoint - checkingMen;
            if(!canAchive(checkingMen, man > 0)) status = Status::Mate;
            if(abs(man) == 3 || abs(man) == 5) {
                if (d.x() > 0 && d.y() > 0){
                    for (int ix = mx + 1, iy = my + 1; ix < kx; ++ix, ++iy)
                        if (canAchive({ix, iy}, man > 0)) return;
                } else if (d.x() > 0 && d.y() < 0){
                    for (int ix = mx + 1, iy = my - 1; ix < kx; ++ix, --iy)
                        if (canAchive({ix, iy}, man > 0)) return;
                } else if (d.x() < 0 && d.y() < 0){
                    for (int ix = kx + 1, iy = ky + 1; ix < mx; ++ix, ++iy)
                        if (canAchive({ix, iy}, man > 0)) return;
                } else if (d.x() < 0 && d.y() > 0)
                    for (int ix = kx + 1, iy = ky - 1; ix < mx; ++ix, --iy)
                        if (canAchive({ix, iy}, man > 0)) return;
            }
            if(abs(man) == 4 || abs(man) == 5) {
                if (d.x() > 0){
                    for (int ix = mx + 1; ix < kx; ++ix)
                        if (canAchive({ix, my}, man > 0)) return;
                } else if (d.x() < 0){
                    for (int ix = kx + 1; ix < mx; ++ix)
                        if (canAchive({ix, ky}, man > 0)) return;
                } else if (d.y() > 0){
                    for (int iy = my + 1; iy < ky; ++iy)
                        if (canAchive({mx, iy}, man > 0)) return;
                } else if (d.y() < 0)
                    for (int iy = ky + 1; iy < my; ++iy)
                        if (canAchive({kx, iy}, man > 0)) return;
            }
        }
    }
    else if (n > 1){
        if (KMove) status = Status::DoubleCheck;
        else status = Status::Mate;
    }
}

bool Board::hasCheck(bool isWhite){
    int n = nChecks(isWhite);
    return n > 0;
}
