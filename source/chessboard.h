#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QLabel>

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    int matrix[8][8]{
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    QLabel **cells;

    explicit ChessBoard(QWidget *parent = nullptr);

signals:

public slots:
    void setBoard(int **men = nullptr);

private:
    int windowSize = 400;
    int nCells = 8;
    int cellSize = 50;

    QString getManPicPath(int m);

    int defMenPosition[8][8]{
        {-4, -2, -3, -5, -6, -3, -2, -4},
        {-1, -1, -1, -1, -1, -1, -1, -1},
        {0 ,  0,  0,  0,  0,  0,  0,  0},
        {0 ,  0,  0,  0,  0,  0,  0,  0},
        {0 ,  0,  0,  0,  0,  0,  0,  0},
        {0 ,  0,  0,  0,  0,  0,  0,  0},
        { 1,  1,  1,  1,  1,  1,  1,  1},
        { 4,  2,  3,  5,  6,  3,  2,  4}
    };
};

#endif // CHESSBOARD_H
