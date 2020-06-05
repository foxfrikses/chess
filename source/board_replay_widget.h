#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H


#include <QWidget>
#include <QLabel>


class BoardReplayWidget : public QWidget
{
    Q_OBJECT

    int windowSize = 400;
    int nCells = 8;
    int cellSize = 50;
public:
    explicit BoardReplayWidget(QWidget *parent = nullptr);
private:
    QString getManPicPath(int m);

signals:
    void sendStatus(int);
    void sendTurn(bool);
public slots:
    void setBoard(int *const *const men = nullptr, int status = 0, bool turn = true);

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
        {-4, -1,  0, 0, 0, 0,  1,  4},
        {-2, -1,  0, 0, 0, 0,  1,  2},
        {-3, -1,  0, 0, 0, 0,  1,  3},
        {-5, -1,  0, 0, 0, 0,  1,  5},
        {-6, -1,  0, 0, 0, 0,  1,  6},
        {-3, -1,  0, 0, 0, 0,  1,  3},
        {-2, -1,  0, 0, 0, 0,  1,  2},
        {-4, -1,  0, 0, 0, 0,  1,  4}
    };
};


#endif // BOARDWIDGET_H
