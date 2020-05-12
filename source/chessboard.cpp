#include "chessboard.h"
#include <QMouseEvent>
#include <QCursor>

ChessBoard::ChessBoard(QWidget *parent) : QWidget(parent) // turn == true - white
{  
    this->setFixedSize(windowSize, windowSize);
    cells = new QLabel*[nCells];
    for (auto i = 0; i < nCells; ++i)
        cells[i] = new QLabel[nCells];
    for (int i = 0; i < nCells; ++i){
        for (int j = (1 + i)%2 ; j < nCells; j += 2)
            cells[i][j].setStyleSheet("QLabel { background-color : gray;}");
        for (int j = (i)%2 ; j < nCells; j += 2)
            cells[i][j].setStyleSheet("QLabel { background-color : white;}");
    }
    for (int i = 0; i < nCells; ++i){
        for (int j = 0; j < nCells; ++j){
            cells[i][j].setParent(this);
            cells[i][j].setGeometry(i * cellSize, j * cellSize, cellSize, cellSize);
            //cells[i][j].setPixmap(QPixmap(getManPicPath(defMenPosition[i][j])));
        }
    }

    setBoard();
}

void ChessBoard::mousePressEvent(QMouseEvent *event){
    QPoint to{event->x()/cellSize, event->y()/cellSize};
    if (moveByClick){
        if (movePoint != to){
            emit sendMove(movePoint, to);
        }
    }
    else{
        if(to.x() < 8 && to.y() < 8 && matrix[to.x()][to.y()] != 0){
            movePoint.setX(to.x());
            movePoint.setY(to.y());
            this->setCursor((QPixmap(getManPicPath(
                            matrix[to.x()][to.y()])).scaled(cellSize, cellSize,
                                                    Qt::KeepAspectRatio)));
        }

    }
}

void ChessBoard::mouseDoubleClickEvent(QMouseEvent *event){
    mousePressEvent(event);
}

void ChessBoard::mouseReleaseEvent(QMouseEvent *event){
    if (moveByClick){
        this->setCursor(QCursor());
        moveByClick = false;
        return;
    }
    QPoint to{event->x()/cellSize, event->y()/cellSize};
    if (movePoint == to){
        if(to.x() < 8 && to.y() < 8 && matrix[to.x()][to.y()] != 0){
            moveByClick = true;
            this->setCursor((QPixmap(getManPicPath(
                                     matrix[to.x()][to.y()])).scaled(cellSize,
                                                  cellSize, Qt::KeepAspectRatio)));

        }
    }
    else{
        emit sendMove(movePoint, to);
        this->setCursor(QCursor());
    }
}

void ChessBoard::setBoard(int *const *const men, int status){
    if (men){
        for (int x = 0; x < nCells; ++x)
            for (int y = 0; y < nCells; ++y)
                if (men[x][y] != matrix[x][y]){
                    matrix[x][y] = men[x][y];
                    cells[x][y].setPixmap(QPixmap(getManPicPath(matrix[x][y])).scaled(cellSize,
                                                                                      cellSize,
                                                                                      Qt::KeepAspectRatio));
                }
    } else
        for (int x = 0; x < nCells; ++x){
            for (int y = 0; y < nCells; ++y)
                if (defMenPosition[x][y] != matrix[x][y]){
                    matrix[x][y] = defMenPosition[x][y];
                    cells[x][y].setPixmap(QPixmap(getManPicPath(matrix[x][y])).scaled(cellSize,
                                                                                       cellSize,
                                                                                       Qt::KeepAspectRatio));
                }
        }
}

QString ChessBoard::getManPicPath(int m)
{
    switch (m) {
        case 1: return QString(":/img/img/pawn-white.png");
        case 2: return QString(":/img/img/knight-white.png");
        case 3: return QString(":/img/img/bishop-white.png");
        case 4: return QString(":/img/img/rook-white.png");
        case 5: return QString(":/img/img/queen-white.png");
        case 6: return QString(":/img/img/king-white.png");

        case -1: return QString(":/img/img/pawn-black.png");
        case -2: return QString(":/img/img/knight-black.png");
        case -3: return QString(":/img/img/bishop-black.png");
        case -4: return QString(":/img/img/rook-black.png");
        case -5: return QString(":/img/img/queen-black.png");
        case -6: return QString(":/img/img/king-black.png");

        case 0:
        default: return nullptr;
    }
}
