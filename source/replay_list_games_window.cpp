#include "replay_list_games_window.h"
#include "ui_replay_list_games_window.h"
#include <QStringListModel>
#include <QDebug>


ReplayListGamesWindow::ReplayListGamesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplayListGamesWindow)
{
    ui->setupUi(this);
}

ReplayListGamesWindow::~ReplayListGamesWindow()
{
    delete ui;
}

void ReplayListGamesWindow::load()
{
    emit loadGames();
}



void ReplayListGamesWindow::getGames(QStringList games)
{
    QStringListModel *model = new QStringListModel();
    model->setStringList(games);
    ui->gamesList->setModel(model);
}

void ReplayListGamesWindow::on_gamesList_clicked(const QModelIndex &index)
{
    qDebug() << "name: " << index.data(Qt::DisplayRole).toString();
    emit loadGame(index.data(Qt::DisplayRole).toString());
    this->hide();
    rgw.exec();
    this->close();
}

void ReplayListGamesWindow::on_exit_button_clicked()
{
    this->close();
}
