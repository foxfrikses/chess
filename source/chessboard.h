#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QLabel>


class ChessBoard : public QWidget
{
    Q_OBJECT

    QPoint movePoint;
    int windowSize = 400;
    int nCells = 8;
    int cellSize = 50;
public:
    explicit ChessBoard(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    QString getManPicPath(int m);

signals:
    void sendMove(const QPoint &from, const QPoint &to);
public slots:
    void setBoard(int *const *const men = nullptr);

public:
    QLabel **cells;
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

private:
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
    bool moveByClick{false};
};



#endif // CHESSBOARD_H
