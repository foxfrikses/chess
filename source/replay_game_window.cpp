#include "replay_game_window.h"
#include "ui_replay_game_window.h"
#include "status.h"
#include <QStringListModel>
#include <QDebug>

ReplayGameWindow::ReplayGameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplayGameWindow)
{
    ui->setupUi(this);
    board = new BoardReplayWidget(ui->board_frame);
    auto geom = board->geometry();
    geom.setX(0); geom.setY(0);
    board->setGeometry(geom);
    connect(board, SIGNAL(sendStatus(int)), this, SLOT(setStatus(int)));
    connect(board, SIGNAL(sendTurn(bool)),   this, SLOT(setTurn(bool)));
}

ReplayGameWindow::~ReplayGameWindow()
{
    delete ui;
    delete board;
}

void ReplayGameWindow::setStatus(int status){
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

void ReplayGameWindow::setTurn(bool turn){
    if (turn) ui->turn->setText(QString("White"));
    else      ui->turn->setText(QString("Black"));
}

void ReplayGameWindow::on_exit_clicked()
{
    this->close();
}

void ReplayGameWindow::getMoves(QStringList moves)
{
    QStringListModel *model = new QStringListModel();
    model->setStringList(moves);
    ui->listView->setModel(model);
}

void ReplayGameWindow::on_listView_clicked(const QModelIndex &index)
{
    qDebug() << "INDEX: " << index.row();
    emit selectedMove(index.row());
}
