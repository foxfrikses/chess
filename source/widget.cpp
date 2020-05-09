#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    board = new ChessBoard(ui->chessboard_frame);
    auto geom = board->geometry();
    geom.setX(0); geom.setY(0);
    board->setGeometry(geom);
}

Widget::~Widget()
{
    delete ui;
}
