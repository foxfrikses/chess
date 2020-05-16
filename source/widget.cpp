#include "widget.h"
#include "ui_widget.h"
#include "status.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    board = new ChessBoard(ui->chessboard_frame);
    auto geom = board->geometry();
    geom.setX(0); geom.setY(0);
    board->setGeometry(geom);
    promDialog.setModal(true);
    connect(this, SIGNAL(promotion(bool)), &promDialog, SLOT(execute(bool)));
    connect(&promDialog, SIGNAL(sendResult(int)),  this, SIGNAL(promotion(int)));
    connect(board, SIGNAL(sendStatus(int)), this, SLOT(setStatus(int)));
    connect(board, SIGNAL(sendTurn(bool)),   this, SLOT(setTurn(bool)));
    connect(ui->newgame, SIGNAL(clicked(bool)), this, SIGNAL(newGame()));
}

Widget::~Widget()
{
    delete ui;
    delete board;
}

void Widget::setStatus(int status){
    switch (Status(status)) {
        case Status::Play:        ui->status->setText(QString("Play"));        break;
        case Status::Mate:        ui->status->setText(QString("Mate"));        break;
        case Status::Check:       ui->status->setText(QString("Check"));       break;
        case Status::DoubleCheck: ui->status->setText(QString("DoubleCheck")); break;
        case Status::StaleMate:   ui->status->setText(QString("StaleMate"));   break;
        case Status::Draw:        ui->status->setText(QString("Draw"));        break;
        case Status::Promotion:                                                break;
    }
}

void Widget::setTurn(bool turn){
    if (turn) ui->turn->setText(QString("White"));
    else      ui->turn->setText(QString("Black"));
}
