#include "chessboard.h"
#include <QMouseEvent>

ChessBoard::ChessBoard(QWidget *parent) : QWidget(parent) // turn == true - white
{  
    this->setFixedSize(windowSize, windowSize);
    cells = new QLabel*[nCells];
    for (auto i{0}; i < nCells; ++i)
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
            cells[i][j].setGeometry(j * cellSize, i * cellSize, cellSize, cellSize);
            //cells[i][j].setPixmap(QPixmap(getManPicPath(defMenPosition[i][j])));
        }
    }

    setBoard();
}

void ChessBoard::mousePressEvent(QMouseEvent *event){
    if (moveByClick){
        QPoint to{event->y()/cellSize, event->x()/cellSize};
        if (movePoint != to){
            emit sendMove(movePoint, to);
        }
    }
    else{
        movePoint.setX(event->y()/cellSize);
        movePoint.setY(event->x()/cellSize);
    }
}

void ChessBoard::mouseDoubleClickEvent(QMouseEvent *event){
    mousePressEvent(event);
}

void ChessBoard::mouseReleaseEvent(QMouseEvent *event){
    if (moveByClick){
        moveByClick = false;
        return;
    }
    QPoint to{event->y()/cellSize, event->x()/cellSize};
    if (movePoint == to){
        moveByClick = true;
    }
    else
        emit sendMove(movePoint, to);
}

void ChessBoard::setBoard(int *const *const men){
    if (men){
        for (int i = 0; i < nCells; ++i)
            for (int j = 0; j < nCells; ++j)
                if (men[i][j] != matrix[i][j]){
                    matrix[i][j] = men[i][j];
                    cells[i][j].setPixmap(QPixmap(getManPicPath(matrix[i][j])).scaled(cellSize,
                                                                                      cellSize,
                                                                                      Qt::KeepAspectRatio));
                }
    } else
        for (int i = 0; i < nCells; ++i){
            for (int j = 0; j < nCells; ++j)
                if (defMenPosition[i][j] != matrix[i][j]){
                    matrix[i][j] = defMenPosition[i][j];
                    cells[i][j].setPixmap(QPixmap(getManPicPath(matrix[i][j])).scaled(cellSize,
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
